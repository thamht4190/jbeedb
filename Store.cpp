#include "Store.h"


Store::Store(Store* parent) : _parent(parent) {}

// part 1
bool Store::insert(int64_t id, const map<string, Variant>& properties)
{
    if (exists(id)) {
        // this id already exists in this store or parent store
        return false;
    }

    Record new_rec(&_shared_keys, properties);
    _records.insert({id, new_rec});

    // for child store, we save the command to replay in parent store later
    if (_parent != nullptr) {
        _pending_changes.push_back({ INSERT, id, properties });
    }
    return true;
}

bool Store::update(int64_t id, const map<string, Variant>& properties)
{
    Record* rec = get_record(id);
    if (rec == nullptr) {
        // this id doesn't exist in store yet
        return false;
    }
    Record updated = *rec;
    updated.update(properties);

    _records.insert_or_assign(id, updated);

    // for child store, we save the command to replay in parent store later
    if (_parent != nullptr) {
        _pending_changes.push_back({ UPDATE, id, properties });
    }
    return true;
}

map<string, Variant> Store::get(int64_t id)
{
    Record* rec = get_record(id);
    if (rec == nullptr) {
        // this id doesn't exist in store yet
        return map<string, Variant>();
    }
    else {
        return rec->get_properties();
    }
}

void Store::remove(int64_t id)
{
    _records.erase(id);

    // for child store, we save the command to replay in parent store later
    if (_parent != nullptr) {
        _pending_changes.push_back({ REMOVE, id, {} });
    }
}

// part 2
set<int64_t> Store::query(const map<string, Variant>& properties)
{
    set<int64_t> result;
    for (auto it = _records.begin(); it != _records.end(); it++) {
        if (it->second.has_properties(properties)) {
            result.insert(it->first);
        }
    }
    // search in parent store as well
    if (_parent != nullptr) {
        set<int64_t> parent_result = _parent->query(properties);
        result.merge(parent_result);

    }
    return result;
}

set<int64_t> Store::range_query(const map<string, pair<Variant,Variant>>& properties)
{
    set<int64_t> result;
    for (auto it = _records.begin(); it != _records.end(); it++) {
        if (it->second.in_range(properties)) {
            result.insert(it->first);
        }
    }
    // search in parent store as well
    if (_parent != nullptr) {
        set<int64_t> parent_result = _parent->range_query(properties);
        result.merge(parent_result);

    }
    return result;
}

// part 3
Store Store::create_child()
{
    return Store(this);
}

void Store::commit()
{
    assert(_parent != nullptr);
    _parent->replay_child_store(this);

    // reset all the changes in this store after commit to parent store
    _records.clear();
    _pending_changes.clear();
}

// return true if this id exists in either this store or parent store
bool Store::exists(int64_t id)
{
    return get_record(id) != nullptr;
}

Record* Store::get_record(int64_t id)
{
    auto it = _records.find(id);
    if (it == _records.end()) {
        if (_parent == nullptr) {
            // this id doesn't exist in both parent store & this store
            return nullptr;
        }
        else {
            // this id may exist in parent store
            return _parent->get_record(id);
        }
    }
    else {
        // if this id exists in this store, we don't need to find in parent store
        return &it->second;
    }
}

void Store::replay_child_store(Store* child)
{
    for (const Command& c : child->_pending_changes) {
        if (c.type == INSERT) {
            insert(c.id, c.data);
        }
        else if (c.type == UPDATE) {
            update(c.id, c.data);
        }
        else if (c.type == REMOVE) {
            remove(c.id);
        }
        else {
            assert(false);
        }
    }
}