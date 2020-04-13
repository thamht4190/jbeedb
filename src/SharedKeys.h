#ifndef SHAREDKEYS_H
#define SHAREDKEYS_H

#include <map>
#include <vector>

using namespace std;

// This class stores the field name mapping with an integer id, starting from 1
// Because the field names will be the same for many records, so using SharedKeys
// will help reduce the stored data size and make comparation faster
class SharedKeys {
public:
    // get key, if it doesn't exist, create one
    int set_key(const string& name)
    {
        auto key = _keys.find(name);
        if (key == _keys.end()) {
            _names.push_back(name);
            _keys.insert({name, _names.size()});
            return _names.size();
        }
        return key->second;
    }

    // return 0 if name doesn't exist
    int key(const string& name)
    {
        auto key = _keys.find(name);
        if (key == _keys.end()) {
            return 0;
        }
        return key->second;
    }

    const string& name(int key) const
    {
        assert(key > 0 && key <= _names.size());
        return _names[key - 1];
    }

private:
    vector<string> _names;
    map<string, int> _keys;
};


#endif // SHAREDKEYS_H