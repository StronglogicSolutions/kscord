#pragma once

#include "third_party/json/include/nlohmann/json.hpp"
#include <string>

namespace kjson {
namespace constants {
const bool JSON_PARSE_NO_THROW{false};
}

inline std::string GetJSONStringValue(const nlohmann::json& data, const std::string& key) {
  if (!data.is_null() && data.contains(key) && !data[key].is_null())
    return data[key].get<std::string>();
  return "";
}

template<typename T>
inline T GetJSONValue(const nlohmann::json& data, const std::string& key) {
  if (!data.is_null() && data.contains(key) && !data[key].is_null())
    return data[key].get<T>();
  if (std::is_integral<T>::value)
    return static_cast<T>(0);
  if (std::is_floating_point<T>::value)
    return static_cast<T>(0);
  // if (std::is_same_v<bool, T>)
  //   return GetJSONBoolValue(data, key);
  if constexpr (std::is_same_v<std::string, T>)
    return GetJSONStringValue(data, key);
  if constexpr (std::is_same_v<std::vector<std::string>, T>) {
    return std::vector<std::string>{};
  }

  throw std::invalid_argument{"Unsupported type"};
}

inline bool GetJSONBoolValue(const nlohmann::json& data, const std::string& key) {
  if (!data.is_null() && data.contains(key) && !data[key].is_null())
    return data[key].get<bool>();
  return "";
}
}