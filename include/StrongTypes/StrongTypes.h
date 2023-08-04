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