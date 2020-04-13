#include "Variant.h"

Variant::Variant() : _type(DataType::UNKNOWN), _val(0) {}
Variant::Variant(double v) : _type(DataType::DOUBLE), _val(v) {}
Variant::Variant(const string& v) : _type(DataType::STRING), _val(new string(v)) {}
Variant::Variant(const char* v) : _type(DataType::STRING), _val(new string(v)) {}

bool Variant::operator==(const Variant& other) const
{
    assert(_type == other._type);
    if (_type == DataType::STRING) {
        return *get<string*>(_val) == *get<string*>(other._val);
    }
    else {
        return _val == other._val;
    }
}

bool Variant::operator!=(const Variant& other) const
{
    assert(_type == other._type);
    if (_type == DataType::STRING) {
        return *get<string*>(_val) != *get<string*>(other._val);
    }
    else {
        return _val != other._val;
    }
}

bool Variant::operator<(const Variant& other)
{
    assert(_type == other._type);
    if (_type == DataType::STRING) {
        return *get<string*>(_val) < *get<string*>(other._val);
    }
    else {
        return _val < other._val;
    }
}

bool Variant::operator>(const Variant& other)
{
    assert(_type == other._type);
    if (_type == DataType::STRING) {
        return *get<string*>(_val) > *get<string*>(other._val);
    }
    else {
        return _val > other._val;
    }
}

string Variant::debug() const
{
    if (_type == DataType::UNKNOWN) return "N/A";
    else if (_type == DataType::DOUBLE) return to_string(get<double>(_val));
    else return "\"" + *get<string*>(_val) + "\"";
}

ostream& operator<<(ostream& out, const Variant& v)
{
    out << v.debug();
    return out;
}