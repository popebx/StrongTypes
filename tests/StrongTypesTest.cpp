#include <StrongTypes/StrongTypes.h>
#include <gtest/gtest.h>

TEST(guid, creation_and_get) {
  guid x{};
  ASSERT_EQ(x.get(), "");
  guid y{"{a1770282-4b4f-4c82-8777-f62f03112281}"};
  ASSERT_EQ(y.get(), "{a1770282-4b4f-4c82-8777-f62f03112281}");
}


TEST(DatabaseId, creation_and_get) {
  DatabaseID id{1};
  ASSERT_EQ(id.get(), 1);
}