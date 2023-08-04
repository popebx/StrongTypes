#pragma once
#include <string>

class guid {
 public:
  guid() noexcept(false);
  explicit guid(std::string_view guid) noexcept(false);

  [[nodiscard]] auto get() const noexcept -> std::string_view;
  [[nodiscard]] auto get() noexcept -> std::string&;
  operator bool() const;

 private:
  std::string data{};
};

class DatabaseID {
 public:
  DatabaseID() = delete;
  explicit DatabaseID(long id) noexcept;
  auto get() noexcept -> long;

  [[nodiscard]] auto operator==(const DatabaseID&) const -> bool = default;
  [[nodiscard]] auto operator!=(const DatabaseID&) const -> bool = default;
  [[nodiscard]] auto operator<=>(const DatabaseID&) const noexcept
      -> std::strong_ordering;

 private:
  long id{};
};

/*
* Basic Idea is to have following Code:
* struct DatabaseIDConfig {
        using underlying_type=long;
        constexpr bool spaceship = true;
        constexpr bool equal = true;
        constexpr bool notEqual = true;
};
using DatabaseId = StrongType<DatabaseIDConfig>;

Problems:
        noexcept => should be inherited from underlying type!
        explicit will be fix for now, since implicit conversion makes no sense
        BinaryLayout should be sizeof(underlying_type) ==
StrongType<underlying_type>; Additional Customizations Points via Inheritance?
Or via compositon?
*
*/

#include <optional>
struct DatabaseIdConfig {
  using underlyingType = long;

  static constexpr bool spaceship = true;
  static constexpr bool equal = true;
  static constexpr bool notEqual = true;

  static constexpr bool lessThen = true;
  static constexpr bool lessEqual = true;
  static constexpr bool greaterThen = true;
  static constexpr bool greaterEqual = true;
};

template <typename config>
concept isStrongTypeConfig = requires() {
  typename config::underlyingType;
  { config::spaceship } -> std::convertible_to<bool>;
  { config::equal } -> std::convertible_to<bool>;
  { config::notEqual } -> std::convertible_to<bool>;

  { config::lessThen } -> std::convertible_to<bool>;
  { config::lessEqual } -> std::convertible_to<bool>;
  { config::greaterThen } -> std::convertible_to<bool>;
  { config::greaterEqual } -> std::convertible_to<bool>;
};

template <isStrongTypeConfig config>
class StrongType {
 public:
  using type = std::remove_cvref_t<typename config::underlyingType>;

  explicit StrongType(const type& in) : data{in} {}
  [[nodiscard]] auto get() noexcept -> type& { return data; }
  [[nodiscard]] auto get() const noexcept -> const type& { return data; }

#pragma region Compare with StrongType <config>
  template <typename otherType>
    requires std::is_same_v<StrongType<config>, otherType> &&
             config::spaceship && std::three_way_comparable<type>
  [[nodiscard]] auto operator<=>(const otherType& rhs) const noexcept {
    return this->data <=> rhs.data;
  }

  template <typename otherType>
    requires std::is_same_v<StrongType<config>, otherType> && config::equal &&
             std::equality_comparable<type>
  [[nodiscard]] auto operator==(const otherType& rhs) const -> bool {
    return this->data == rhs.data;
  };
  template <typename otherType>
    requires std::is_same_v<StrongType<config>, otherType> &&
             config::notEqual && std::equality_comparable<type>
  [[nodiscard]] auto operator!=(const otherType& rhs) const -> bool {
    return this->data != rhs.data;
  };

  template <typename otherType>
    requires std::is_same_v<StrongType<config>, otherType> &&
             (config::lessThen && !config::spaceship)
  [[nodiscard]] auto operator<(const otherType& rhs) const -> bool {
    return this->data < rhs.data;
  };
  template <typename otherType>
    requires std::is_same_v<StrongType<config>, otherType> &&
             (config::lessEqual && !config::spaceship)
  [[nodiscard]] auto operator<=(const otherType& rhs) const -> bool {
    return this->data <= rhs.data;
  };
  template <typename otherType>
    requires std::is_same_v<StrongType<config>, otherType> &&
             (config::greaterThen && !config::spaceship)
  [[nodiscard]] auto operator>(const otherType& rhs) const -> bool {
    return this->data > rhs.data;
  };
  template <typename otherType>
    requires std::is_same_v<StrongType<config>, otherType> &&
             (config::greaterEqual && !config::spaceship)
  [[nodiscard]] auto operator>=(const otherType& rhs) const -> bool {
    return this->data >= rhs.data;
  };
#pragma endregion
#pragma region Compare with underlying Type
  /*
  template <typename otherType>
    requires std::is_same_v<StrongType<config>, otherType> &&
                     (config::spaceship &&
  config::allowUnderlyingTypeInOperator) && std::three_way_comparable<type>
  [[nodiscard]] auto operator<=>(const otherType& rhs) const noexcept {
    return this->data <=> rhs.data;
  }

  template <typename otherType>
    requires std::is_same_v<type, otherType> &&
                     (config::equal && config::allowUnderlyingTypeInOperator)
                     std::equality_comparable<type> [[nodiscard]] auto
  operator==( const otherType& rhs) const -> bool { return this->data == rhs;
  };
  template <typename otherType>
    requires std::is_same_v<StrongType<config>, otherType> &&
                     config::notEqual &&
                     std::equality_comparable<type>
                     [[nodiscard]] auto operator!=(const otherType& rhs) const
  -> bool { return this->data != rhs.data;
  };

  template <typename otherType>
    requires std::is_same_v<, otherType> &&
                     (config::lessThen && !config::spaceship)
                     [[nodiscard]] auto operator<(const otherType& rhs) const
  -> bool { return this->data < rhs.data;
  };
  template <typename otherType>
    requires std::is_same_v<StrongType<config>, otherType> &&
                     (config::lessEqual && !config::spaceship)
                     [[nodiscard]] auto operator<=(const otherType& rhs) const
  -> bool { return this->data <= rhs.data;
  };
  template <typename otherType>
    requires std::is_same_v<StrongType<config>, otherType> &&
                     (config::greaterThen && !config::spaceship)
                     [[nodiscard]] auto operator>(const otherType& rhs) const
  -> bool { return this->data > rhs.data;
  };
  template <typename otherType>
    requires std::is_same_v<StrongType<config>, otherType> &&
                     (config::greaterEqual && !config::spaceship)
                     [[nodiscard]] auto operator>=(const otherType& rhs) const
  -> bool { return this->data >= rhs.data;
  };*/
#pragma endregion

 private:
  type data;
};

using DbId = StrongType<DatabaseIdConfig>;
