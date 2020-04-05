#include <cassert>
#include <iostream>

#include <string>
#include <map>
#include <vector>
#include <set>
#include "Store.h"
#include "Variant.h"

using namespace std;

int main(int argc, const char *argv[])
{
    Store store;
    
    const int64_t id = 2113;
    map<string, Variant> props {
        { "title", "Buy Milk" }, 
        { "description", "made of almonds!" },
        { "timestamp", 2392348.12233 }};
    
    // ------- Part 1 - A Basic Store-------
    {
        // Insert a new entity
        store.insert(id, props);

        // Update only specified properties
        map<string, Variant> new_props {{ "title", "Buy Chocolate" }};
        store.update(id, new_props);

        // Retrieve properties of an entity
        map<string, Variant> updated_props = store.get(id);

        assert(updated_props["title"] == new_props["title"]);

        // Remove an entity with given id
        store.remove(id);
        assert(store.get(id).empty());
    }

    // ------- Part 2 – Queries -------
    {
        store.insert(id, props);

        set<int64_t> result_query = store.query({{ "title", "Buy Milk" }});
        assert(result_query.size() == 1);
        assert(result_query.find(id) != result_query.end());

        set<int64_t> result_range_query = store.range_query({{"timestamp", {1000, 2300}}});
        assert(result_range_query.size() == 0);
    }


    // ------- Part 3 – Child Stores -------
    {
        store.insert(id, props);

        // 1. Create a child store
        Store child = store.create_child();

        // 2. Use it to insert, get, update, delete, and query entities
        map<string, Variant> p1 = child.get(id);
        assert(p1 == props);

        child.update(id, {{ "title", "Buy Cream" }});

        map<string, Variant> p2 = child.get(id);
        assert(p2["title"] == "Buy Cream");

        map<string, Variant> p3 = store.get(id);
        assert(p3["title"] == props["title"]);

        set<int64_t> ids = child.query({{"title", "Buy Cream"}});
        assert(ids.size() == 1);
        assert(ids.find(id) != ids.end());

        // 3. Remove the child store and commit its changes to the parent

        child.commit();

        map<string, Variant> p4 = store.get(id);
        assert(p4["title"] == "Buy Cream");
    }

    // ------- Part 4 – Performance Considerations -------
    // I can think of some ways to improve query performance:
    // 1. Indexing
    // - We can create index on one or several specific fields (e.g: "title"). With a field value (e.g: "Buy Cream"),
    // we can access all records having that field value faster.
    // - Indexed field value are ordered then we can find a field value in index faster
    // 2. We can divide main store into multiple buckets, each bucket for a Record type, so we can query only on that bucket
    // 3. We can create some view that caches query result, next time we can query faster.

    return 0;
}