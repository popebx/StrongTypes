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

TEST(DbId, creation_and_get) {
  DbId id{1};
  ASSERT_EQ(id.get(), 1);
}

TEST(DbId, spaceship) {
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

struct AnotherDatabaseIdConfig {
  using underlyingType = long;

  static constexpr bool spaceship = true;
  static constexpr bool equal = true;
  static constexpr bool notEqual = true;

  static constexpr bool lessThen = true;
  static constexpr bool lessEqual = true;
  static constexpr bool greaterThen = true;
  static constexpr bool greaterEqual = true;

  static constexpr bool allowUnderlyingTypeInOperator = true;
};

using DbId2 = StrongType<AnotherDatabaseIdConfig>;
static_assert(!std::is_same_v<DbId2, DbId>,
              "Both strong types shouldn't be the same");

static_assert(!std::equality_comparable_with<DbId, DbId2>);
static_assert(!std::three_way_comparable_with<DbId, DbId2>);

struct OldTypeConfig {
  using underlyingType = int;

  static constexpr bool spaceship = false;
  static constexpr bool equal = true;
  static constexpr bool notEqual = true;

  static constexpr bool lessThen = true;
  static constexpr bool lessEqual = true;
  static constexpr bool greaterThen = true;
  static constexpr bool greaterEqual = true;
  static constexpr bool allowUnderlyingTypeInOperator = true;
};

using OldType = StrongType<OldTypeConfig>;

TEST(OldType, creation_and_get) {
  OldType id{1};
  ASSERT_EQ(id.get(), 1);
}

TEST(OldType, OldOperator) {
  OldType first{1};
  OldType second{2};
  ASSERT_EQ(first, first);
  ASSERT_NE(first, second);
  ASSERT_LE(first, first);
  ASSERT_LE(first, second);
  ASSERT_LT(first, second);
  ASSERT_GT(second, first);
  ASSERT_GE(second, second);
  ASSERT_GE(second, first);
}
TEST(OldType, UnderlyingTypeOperator) {
  OldType first{1};
  OldType second{2};
  ASSERT_EQ(first, 1);
  ASSERT_NE(first, 2);
  ASSERT_LE(first, 1);
  ASSERT_LE(first, 2);
  ASSERT_LT(first, 2);
  ASSERT_GT(second, 1);
  ASSERT_GE(second, 2);
  ASSERT_GE(second, 1);
}

struct NoSpaceship {
  using underlyingType = int;

  static constexpr bool spaceship = false;
  static constexpr bool equal = true;
  static constexpr bool notEqual = true;

  static constexpr bool lessThen = true;
  static constexpr bool lessEqual = true;
  static constexpr bool greaterThen = true;
  static constexpr bool greaterEqual = true;
  static constexpr bool allowUnderlyingTypeInOperator = true;
};
static_assert(!isSpaceshipComparable<StrongType<NoSpaceship>>);
struct NoEqual {
  using underlyingType = int;

  static constexpr bool spaceship = true;
  static constexpr bool equal = false;
  static constexpr bool notEqual = true;

  static constexpr bool lessThen = true;
  static constexpr bool lessEqual = true;
  static constexpr bool greaterThen = true;
  static constexpr bool greaterEqual = true;
  static constexpr bool allowUnderlyingTypeInOperator = true;
};
static_assert(!isEqualComparable<StrongType<NoEqual>>);
struct NoNotEqual {
  using underlyingType = int;

  static constexpr bool spaceship = true;
  static constexpr bool equal = true;
  static constexpr bool notEqual = false;

  static constexpr bool lessThen = true;
  static constexpr bool lessEqual = true;
  static constexpr bool greaterThen = true;
  static constexpr bool greaterEqual = true;
  static constexpr bool allowUnderlyingTypeInOperator = true;
};
static_assert(!isNotEqualComparable<StrongType<NoNotEqual>>);
struct NoLessThen {
  using underlyingType = int;

  static constexpr bool spaceship = false;
  static constexpr bool equal = true;
  static constexpr bool notEqual = true;

  static constexpr bool lessThen = false;
  static constexpr bool lessEqual = true;
  static constexpr bool greaterThen = true;
  static constexpr bool greaterEqual = true;
  static constexpr bool allowUnderlyingTypeInOperator = true;
};
static_assert(!isLessThenComparable<StrongType<NoLessThen>>);
struct NoLessEqual {
  using underlyingType = int;

  static constexpr bool spaceship = false;
  static constexpr bool equal = true;
  static constexpr bool notEqual = true;

  static constexpr bool lessThen = true;
  static constexpr bool lessEqual = false;
  static constexpr bool greaterThen = true;
  static constexpr bool greaterEqual = true;
  static constexpr bool allowUnderlyingTypeInOperator = true;
};
static_assert(!isLessEqualComparable<StrongType<NoLessEqual>>);
struct NoGreaterThen {
  using underlyingType = int;

  static constexpr bool spaceship = false;
  static constexpr bool equal = true;
  static constexpr bool notEqual = true;

  static constexpr bool lessThen = true;
  static constexpr bool lessEqual = true;
  static constexpr bool greaterThen = false;
  static constexpr bool greaterEqual = true;
  static constexpr bool allowUnderlyingTypeInOperator = true;
};
static_assert(!isGreaterThenComparable<StrongType<NoGreaterThen>>);
struct NoGreaterEqual {
  using underlyingType = int;

  static constexpr bool spaceship = false;
  static constexpr bool equal = true;
  static constexpr bool notEqual = true;

  static constexpr bool lessThen = true;
  static constexpr bool lessEqual = true;
  static constexpr bool greaterThen = true;
  static constexpr bool greaterEqual = false;
  static constexpr bool allowUnderlyingTypeInOperator = true;
};
static_assert(!isGreaterEqualComparable<StrongType<NoGreaterEqual>>);

struct NoUnderlyingTypeComparision {
  using underlyingType = int;

  static constexpr bool spaceship = false;
  static constexpr bool equal = true;
  static constexpr bool notEqual = true;

  static constexpr bool lessThen = true;
  static constexpr bool lessEqual = true;
  static constexpr bool greaterThen = true;
  static constexpr bool greaterEqual = true;
  static constexpr bool allowUnderlyingTypeInOperator = false;
};
static_assert(!isSpaceshipComparable<StrongType<NoUnderlyingTypeComparision>, int>);
static_assert(!isEqualComparable<StrongType<NoUnderlyingTypeComparision>, int>);
static_assert(!isNotEqualComparable<StrongType<NoUnderlyingTypeComparision>, int>);

static_assert(!isNotEqualComparable<StrongType<NoUnderlyingTypeComparision>, int>);
static_assert(!isNotEqualComparable<StrongType<NoUnderlyingTypeComparision>, int>);
static_assert(!isNotEqualComparable<StrongType<NoUnderlyingTypeComparision>, int>);
static_assert(!isNotEqualComparable<StrongType<NoUnderlyingTypeComparision>, int>);
