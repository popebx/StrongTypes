#include <StrongTypes/StrongTypes.h>

guid::guid() {}

guid::guid(std::string_view guid) : data{guid} {}

auto guid::get() const -> std::string_view {
  return data;
}

DatabaseID::DatabaseID(long newId) : id{newId} {}

auto DatabaseID::get() -> long {
  return id;
}
