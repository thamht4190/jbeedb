#ifndef VARIANT_H
#define VARIANT_H

#include <iostream>
#include <string>
#include <variant>

using namespace std;

enum class DataType : char {
    UNKNOWN,
    DOUBLE,
    STRING
};

class Variant {
public:
    Variant();
    Variant(double v);
    Variant(const string& v);
    Variant(const char* v);

    bool operator==(const Variant& other) const;
    bool operator!=(const Variant& other) const;
    bool operator<(const Variant& other);
    bool operator>(const Variant& other);

    string debug() const;

private:
    DataType _type;
    variant<string*, double> _val;
};

ostream& operator<<(ostream& out, const Variant& v);

#endif // VARIANT_H