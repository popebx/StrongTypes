#include <StrongTypes/StrongTypes.h>
#include <gtest/gtest.h>

TEST(guid, creation_and_get) {
  guid x{};
  ASSERT_NE(x.get(), "");
  guid y{"{a1770282-4b4f-4c82-8777-f62f03112281}"};
  ASSERT_EQ(y.get(), "{a1770282-4b4f-4c82-8777-f62f03112281}");
}

TEST(guid, validation) {
  guid x{};
  ASSERT_FALSE(x);
  x = guid{"{a1770282-4b4f-4c82-8777-f62f03112281}"};
  ASSERT_TRUE(x);
}

TEST(DatabaseId, creation_and_get) {
  DatabaseID id{1};
  ASSERT_EQ(id.get(), 1);
}

TEST(DatabaseId, spaceship) {
  DatabaseID first{1};
  DatabaseID second{2};
  ASSERT_EQ(first, first);
  ASSERT_NE(first, second);
  ASSERT_LE(first, first);
  ASSERT_LE(first, second);
  ASSERT_LT(first, second);
  ASSERT_GT(second, first);
  ASSERT_GE(second, second);
  ASSERT_GE(second, first);
}


TEST(DatabaseId, creation_and_get) {
  DbId id{1};
  ASSERT_EQ(id.get(), 1);
}

TEST(DatabaseId, spaceship) {
  DbId first{1};
  DbId second{2};
  ASSERT_EQ(first, first);
  ASSERT_NE(first, second);
  ASSERT_LE(first, first);
  ASSERT_LE(first, second);
  ASSERT_LT(first, second);
  ASSERT_GT(second, first);
  ASSERT_GE(second, second);
  ASSERT_GE(second, first);
}