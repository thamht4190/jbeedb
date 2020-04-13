#include <gtest/gtest.h>
#include <map>
#include <Store.h>

using namespace std;

namespace test {

class StoreTest : public ::testing::Test {
protected:
    StoreTest() {
     // You can do set-up work for each test here.
    }

    ~StoreTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
    }

    void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
    }

    // Class members declared here can be used by all tests in the test suite
};

TEST(StoreTest, CRUD) {
    Store store;
    
    const int64_t id = 2113;
    map<string, Variant> props {
        { "title", "Buy Milk" }, 
        { "description", "made of almonds!" },
        { "timestamp", 2392348.12233 }};
    
    // Insert a new entity
    store.insert(id, props);

    // Update only specified properties
    map<string, Variant> new_props {{ "title", "Buy Chocolate" }};
    store.update(id, new_props);

    // Retrieve properties of an entity
    map<string, Variant> updated_props = store.get(id);

    EXPECT_EQ(updated_props["title"], new_props["title"]);

    // Remove an entity with given id
    store.remove(id);
    EXPECT_EQ(store.get(id).empty(), true);
}

TEST(StoreTest, query) {
    Store store;
    
    const int64_t id = 2113;
    map<string, Variant> props {
        { "title", "Buy Milk" }, 
        { "description", "made of almonds!" },
        { "timestamp", 2392348.12233 }};

    store.insert(id, props);

    set<int64_t> result_query = store.query({{ "title", "Buy Milk" }});
    EXPECT_EQ(result_query.size(), 1);
    EXPECT_NE(result_query.find(id), result_query.end());

    set<int64_t> result_range_query = store.range_query({{"timestamp", {1000, 2300}}});
    EXPECT_EQ(result_range_query.size(), 0);
}

TEST(StoreTest, childStore) {
    Store store;
    
    const int64_t id = 2113;
    map<string, Variant> props {
        { "title", "Buy Milk" }, 
        { "description", "made of almonds!" },
        { "timestamp", 2392348.12233 }};

    store.insert(id, props);

    // 1. Create a child store
    Store child = store.create_child();

    // 2. Use it to insert, get, update, delete, and query entities
    map<string, Variant> p1 = child.get(id);
    EXPECT_EQ(p1, props);

    child.update(id, {{ "title", "Buy Cream" }});

    map<string, Variant> p2 = child.get(id);
    EXPECT_EQ(p2["title"], "Buy Cream");

    map<string, Variant> p3 = store.get(id);
    EXPECT_EQ(p3["title"], props["title"]);

    set<int64_t> ids = child.query({{"title", "Buy Cream"}});
    EXPECT_EQ(ids.size(), 1);
    EXPECT_NE(ids.find(id), ids.end());

    // 3. Remove the child store and commit its changes to the parent

    child.commit();

    map<string, Variant> p4 = store.get(id);
    EXPECT_EQ(p4["title"], "Buy Cream");
}

} // namespace test

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}