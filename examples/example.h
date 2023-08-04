#pragma once
#include <Objbase.h>
#include <StrongTypes/StrongTypes.h>
#include <string>
#include <stdexcept>
#include <regex>


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

struct Guid2Config {
  using underlyingType = std::string;

  static constexpr bool spaceship = false;
  static constexpr bool equal = false;
  static constexpr bool notEqual = false;

  static constexpr bool lessThen = false;
  static constexpr bool lessEqual = false;
  static constexpr bool greaterThen = false;
  static constexpr bool greaterEqual = false;

  static constexpr bool allowUnderlyingTypeInOperator = false;
};

class guid2 : public StrongType<Guid2Config> {
 public:
  guid2() : StrongType{""} {
    GUID guid;
    if (CoCreateGuid(&guid) != S_OK) {
      throw std::logic_error("Can't create GUID!");
    }
    wchar_t* guidString = nullptr;
    if (StringFromCLSID(guid, &guidString) != S_OK) {
      throw std::logic_error("Can't create String from GUID!");
    }

    auto size = WideCharToMultiByte(CP_UTF8, 0, guidString, -1, nullptr, 0,
                                    nullptr, nullptr);
    if (size <= 0) {
      throw std::logic_error("Error converting to UTF-8");
    }
    data.resize(size);
    WideCharToMultiByte(CP_UTF8, 0, guidString, -1, data.data(), data.size(),
                        nullptr, nullptr);
    CoTaskMemFree(guidString);
  }
  using StrongType::StrongType;
  operator bool() const {
    const std::regex r{
        "^[{]?[0-9a-fA-F]{8}-([0-9a-fA-F]{4}-){3}[0-9a-fA-F]{12}[}]?$"};
    return std::regex_match(this->data, r);
  }
};