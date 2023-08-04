#pragma once
#include <string>

class guid {
 public:
  guid() noexcept(false);
  explicit guid(std::string_view guid);

  [[nodiscard]] auto get() const -> std::string_view;
  [[nodiscard]] auto get() -> std::string&;
  operator bool() const;
 private:
  std::string data{};
};

class DatabaseID {
 public:
  DatabaseID() = delete;
  explicit DatabaseID(long id);
  auto get() -> long;

  [[nodiscard]] auto operator==(const DatabaseID&) const -> bool = default;
  [[nodiscard]] auto operator!=(const DatabaseID&) const -> bool = default;
  [[nodiscard]] std::strong_ordering operator<=>(const DatabaseID&) const;
 private:
  long id{};
};