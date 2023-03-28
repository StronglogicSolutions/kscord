#include "util.hpp"

namespace kscord {
/**
 * SaveToFile
 */
void SaveToFile(std::string data, std::string path) {
  std::ofstream o{path};
  o << data;
}

/**
 * SaveToFile
 */
void SaveToFile(nlohmann::json data, std::string path)
{
  std::ofstream o{path};
  o << data;
}

/**
 * ReadFromFile
 */
std::string ReadFromFile(std::string path) {
  std::ifstream f{path};
  std::stringstream fs{};
  fs << f.rdbuf();
  return fs.str();
}

/**
 * ReadBytesFromFile
 *
 * @param path
 * @return std::vector<uint8_t>
 */
std::vector<uint8_t> ReadBytesFromFile(std::string path) {
  std::string file = ReadFromFile(path);

  return std::vector<uint8_t>{file.begin(), file.end()};
}

nlohmann::json LoadJSONFile(std::string path) {

  return nlohmann::json::parse(ReadFromFile(path), nullptr, false);
}

/**
 * Poor man's log
 *
 * @param
 */
template<typename T>
void log(T s) {
  std::cout << s << std::endl;
}

std::string SanitizeOutput(const std::string& s) {
  std::string o{};

  for (const char& c : s) {
    if (c == '\'')
      o += "\'";
    // else
    // if (c == '"')
    //   o += "\\\"";
    else
    if (c == '(')
      o += "&#x28;";
    else
    if (c == ')')
      o += "&#x29;";
    else
      o += c;
  }

  return o;
}

template void kscord::log<char const*>(char const*);
template void kscord::log<std::string>(std::string);

/**
 * SanitizeJSON
 *
 * Helper function to remove escaped double quotes from a string
 *
 * @param   [in] {std::string}
 * @returns [in] {std::string}
 */
std::string SanitizeJSON(std::string s) {
  s.erase(
    std::remove(s.begin(), s.end(),'\"'),
    s.end()
  );
  return s;
}

/**
 * SanitizeInput
 *
 * Helper function to remove quotes from a string
 *
 * @param   [in] {std::string}
 * @returns [in] {std::string}
 */
std::string SanitizeInput(std::string s) {
  s.erase(
    std::remove_if(s.begin(), s.end(), [](char c){
      return c == '\'' || c == '\"';
    }),
  s.end());

  return s;
}

/**
 * SanitizeJSON
 *
 * Helper function to remove escaped double quotes from a string
 *
 * @param [in] {std::string&} A reference to a string object
 */
std::string StripLineBreaks(std::string s) {
  s.erase(
    std::remove(s.begin(), s.end(),'\n'),
    s.end()
  );

  return s;
}

/**
 * CreateStringWithBreaks
 *
 * @param
 * @param
 * @returns
 */
std::string CreateStringWithBreaks(const std::string &in, const size_t every_n) {
  std::string out{};
  out.reserve(in.size() + in.size() / every_n);
  for(std::string::size_type i = 0; i < in.size(); i++) {
    (isascii(static_cast<uint8_t>(in[i]))) ?
    (!(i % every_n) && i) ? out.push_back('\n') : out.push_back(in[i]) :
    (void)(out);
  }
  return out;
}

uint64_t string_to_uint64(const std::string& s) {
  uint64_t    value{};
  const char* ptr = s.c_str();
  const char* termination_character = ptr + s.size();

  while (ptr < termination_character) {
    value = (value << 1) + (value << 3) + *(ptr++) - '0';
  }

  return value;
}

std::string FetchTemporaryFile(const std::string& url)
{
  const std::string filename = url.substr(url.find_last_of('/') + 1);
  cpr::Response r = cpr::Get(cpr::Url{url});
  SaveToFile(r.text, filename);

  return filename;
}

void EraseFile(const std::string& path)
{
  std::remove(path.c_str());
}

} // namespace kscord