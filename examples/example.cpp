#include "example.h"

guid::guid() {
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

guid::guid(std::string_view guid) : data{guid} {}

auto guid::get() const noexcept -> std::string_view {
  return data;
}

auto guid::get() noexcept -> std::string& {
  return data;
}

guid::operator bool() const {
  const std::regex r{
      "^[{]?[0-9a-fA-F]{8}-([0-9a-fA-F]{4}-){3}[0-9a-fA-F]{12}[}]?$"};
  return std::regex_match(data, r);
}

int main() {
  guid x();
  guid2 y{};
}