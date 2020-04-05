#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <map>
#include "SharedKeys.h"
#include "Variant.h"

using namespace std;

class Record {
public:
    Record(SharedKeys* shared_keys, const map<string, Variant>& properties);
    
    void update(const map<string, Variant>& properties);
    map<string, Variant> get_properties();
    bool has_properties(const map<string, Variant>& properties);
    bool in_range(const map<string, pair<Variant,Variant>>& properties);

private:
    SharedKeys* _shared_keys;
    map<int, Variant> _properties;
};

ostream& operator<<(ostream& out, const map<string, Variant>& props);

#endif // RECORD_H