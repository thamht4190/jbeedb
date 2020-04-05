#ifndef STORE_H
#define STORE_H

#include <map>
#include <set>
#include <vector>
#include "Record.h"
#include "SharedKeys.h"
#include "Variant.h"

using namespace std;

class Store {
public:
    Store(Store* parent = nullptr);

    // basic operations
    bool insert(int64_t id, const map<string, Variant>& properties);
    bool update(int64_t id, const map<string, Variant>& properties);
    map<string, Variant> get(int64_t id);
    void remove(int64_t id);

    // query
    set<int64_t> query(const map<string, Variant>& properties);
    set<int64_t> range_query(const map<string, pair<Variant,Variant>>& properties);

    // child store
    Store create_child();
    void commit();

private:
    // return true if this id exists in either this store or parent store
    bool exists(int64_t id);
    Record* get_record(int64_t id);
    void replay_child_store(Store* child);

    SharedKeys _shared_keys;
    map<int64_t, Record> _records;
    // for child store
    // child store will have non-null parent and pending changes to be committed into parent store
    Store* _parent;
    enum {
        INSERT = 0,
        UPDATE,
        REMOVE
    };
    struct Command {
        char type;
        int64_t id;
        map<string, Variant> data;
    };
    vector<Command> _pending_changes;
    
};

#endif // STORE_H