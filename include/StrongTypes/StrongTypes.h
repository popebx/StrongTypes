#pragma once
#include <string>

class guid {
 public:
  guid();
  explicit guid(std::string_view guid);

  [[nodiscard]] auto get() const -> std::string_view;

 private:
  std::string data{};
};

class DatabaseID {
 public:
  DatabaseID() = delete;
  explicit DatabaseID(long id);
  auto get() -> long;

 private:
  long id{};
};