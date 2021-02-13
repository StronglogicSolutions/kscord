#pragma once

#include <ctime>
#include <iomanip>
#include <cstdio>
#include <ctype.h>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>

#include "types.hpp"

namespace kscord {
/**
 * SaveToFile
 */
void SaveToFile(std::string data, std::string path);

/**
 * SaveToFile
 */
void SaveToFile(nlohmann::json data, std::string path);

/**
 * ReadFromFile
 */
std::string ReadFromFile(std::string path);

/**
 * ReadBytesFromFile
 *
 * @param path
 * @return std::vector<uint8_t>
 */
std::vector<uint8_t> ReadBytesFromFile(std::string path);

nlohmann::json LoadJSONFile(std::string path);

/**
 * Poor man's log
 *
 * @param
 */
template<typename T>
void log(T s);

std::string SanitizeOutput(const std::string& s);

/**
 * SanitizeJSON
 *
 * Helper function to remove escaped double quotes from a string
 *
 * @param   [in] {std::string}
 * @returns [in] {std::string}
 */
std::string SanitizeJSON(std::string s);

/**
 * SanitizeInput
 *
 * Helper function to remove quotes from a string
 *
 * @param   [in] {std::string}
 * @returns [in] {std::string}
 */
std::string SanitizeInput(std::string s);

/**
 * SanitizeJSON
 *
 * Helper function to remove escaped double quotes from a string
 *
 * @param [in] {std::string&} A reference to a string object
 */
std::string StripLineBreaks(std::string s);

/**
 * CreateStringWithBreaks
 *
 * @param
 * @param
 * @returns
 */
std::string CreateStringWithBreaks(const std::string &in, const size_t every_n);
uint64_t    string_to_uint64(const std::string& s);
std::string FetchTemporaryFile(const std::string& url);
void        EraseFile(const std::string& path);

} // namespace kscord