#include "Record.h"

Record::Record(SharedKeys* shared_keys, const map<string, Variant>& properties)
    : _shared_keys(shared_keys)
{
    update(properties);
}

void Record::update(const map<string, Variant>& properties)
{ 
    for (auto it = properties.begin(); it != properties.end(); it++) {
        int key = _shared_keys->set_key(it->first);
        _properties.insert_or_assign(key, it->second);
    }
}

map<string, Variant> Record::get_properties()
{
    map<string, Variant> result;
    for (auto it = _properties.begin(); it != _properties.end(); it++) {
        const string& name = _shared_keys->name(it->first);
        result.insert({name, it->second});
    }
    return result;
}

bool Record::has_properties(const map<string, Variant>& properties)
{
    for (auto it = properties.begin(); it != properties.end(); it++) {
        int key = _shared_keys->key(it->first);
        if (key == 0) {
            return false;
        }
        auto v = _properties.find(key);
        if (v == _properties.end() || v->second != it->second) {
            return false;
        }
    }
    return true;
}

bool Record::in_range(const map<string, pair<Variant,Variant>>& properties) {
    for (auto it = properties.begin(); it != properties.end(); it++) {
        int key = _shared_keys->key(it->first);
        if (key == 0) {
            return false;
        }
        Variant min = it->second.first;
        Variant max = it->second.second;
        
        auto rec = _properties.find(key);
        if (rec == _properties.end() || rec->second < min || rec->second > max) {
            return false;
        }
    }
    return true;
}

ostream& operator<<(ostream& out, const map<string, Variant>& props)
{
    out << "{\n";
    for (auto it = props.begin(); it != props.end(); it++) {
        out << "\t\"" << it->first << "\" => " << it->second << ",\n";
    }
    out << "}" << endl;
    return out;
}