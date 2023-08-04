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

struct DatabaseIdConfig {
  using underlyingType = long;

  static constexpr bool spaceship = true;
  static constexpr bool equal = true;
  static constexpr bool notEqual = true;
};

template<typename config>
class StrongType {

};

using DbId = StrongType<DatabaseIdConfig>;