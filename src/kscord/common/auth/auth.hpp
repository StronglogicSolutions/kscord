#pragma once

#include <INIReader.h>
#include "kscord/common/request.hpp"
#include "kscord/common/discord_util.hpp"

namespace kscord {
inline bool ValidateCredentialsJSON(nlohmann::json json_file) {
  return(
    !json_file.is_null()                &&
    json_file.is_object()               &&
    json_file.contains("scope")         &&
    json_file.contains("client_id")     &&
    json_file.contains("client_secret") &&
    json_file.contains("public_key")     &&
    json_file.contains("code")
  );
}

inline bool JSONHasUser(nlohmann::json data, std::string username) {
  return (!data.is_null() && data.is_object() && data.contains(username));
}

inline Credentials ParseCredentialsFromJSON(nlohmann::json json_file, std::string username) {
  using namespace kjson;
  Credentials creds{};
  if (
    JSONHasUser(json_file, username) &&
    ValidateCredentialsJSON(json_file[username])) {

    nlohmann::json user_json = json_file[username];

    creds.scope        =  GetJSONStringValue(user_json, "scope");
    creds.client_id =     GetJSONStringValue(user_json, "client_id");
    creds.client_secret = GetJSONStringValue(user_json, "client_secret");
    creds.public_key =     GetJSONStringValue(user_json, "public_key");
    creds.code =          GetJSONStringValue(user_json, "code");
  }

  return creds;
}

inline bool ValidateAuthJSON(const nlohmann::json& json_file) {
  return(
    !json_file.is_null()               &&
    json_file.is_object()              &&
    json_file.contains("access_token") &&
    json_file.contains("token_type")   &&
    json_file.contains("scope")        &&
    json_file.contains("expires_in")
  );
}

// inline std::string AuthToJSON(Auth auth) {
//   nlohmann::json auth_json{};
//   auth_json["access_token"] = auth.access_token;
//   auth_json["token_type"]   = auth.token_type;
//   auth_json["scope"]        = auth.scope;
//   auth_json["created_at"]   = auth.created_at;

//   return auth_json.dump();
// }

inline std::vector<BotInfo> ParseBotInfoFromJSON(const nlohmann::json& data)
{
  using namespace kjson;

  std::vector<BotInfo> info{};

  if (!data.is_null() && data.is_object())
    for (const auto& item : data.items())
      info.emplace_back(BotInfo{
        .name = item.key(),
        .token = item.value()
      });

  return info;
}

inline Auth ParseAuthFromJSON(nlohmann::json json_file) {
  using namespace kjson;

  Auth auth{};

  if (ValidateAuthJSON(json_file)) {
    auth.access_token =  GetJSONStringValue(json_file, "access_token");
    auth.refresh_token = GetJSONStringValue(json_file, "refresh_token");
    auth.token_type   =  GetJSONStringValue(json_file, "token_type");
    auth.scope        =  GetJSONStringValue(json_file, "scope");
    auth.expires_in   =  std::to_string(GetJSONValue<uint32_t>(json_file, "expires_in"));
    auth.bots         =  ParseBotInfoFromJSON(json_file["bots"]);
  }

  return auth;
}




class Authenticator {

public:

Authenticator(std::string username = "")
: m_username(username),
  m_authenticated(false)
{
  auto config = GetConfigReader();

  if (m_username.empty()) {

    if (config.ParseError() < 0) {
      kscord::log("Error loading config");
      throw std::invalid_argument{"No configuration path"};
    }

    auto name = config.GetString(constants::KSCORD_SECTION, constants::USER_CONFIG_KEY, "");

    if (name.empty()) {
      throw std::invalid_argument{"No username in config. Please provide a username"};
    }

    m_username = name;
  }

  auto creds_path = config.GetString(constants::KSCORD_SECTION, constants::CREDS_PATH_KEY, "");

  if (!creds_path.empty())
    m_credentials = ParseCredentialsFromJSON(LoadJSONFile(creds_path), m_username);

  auto m_tokens_path = config.GetString(constants::KSCORD_SECTION, constants::TOKENS_PATH_KEY, "");

  if (!m_tokens_path.empty())
    m_token_json = LoadJSONFile(m_tokens_path);

  if (m_token_json.contains(m_username) && !m_token_json[m_username].is_null()) {
    auto auth = ParseAuthFromJSON(m_token_json[m_username]);

    if (auth.is_valid()) {
      m_auth          = auth;
      m_authenticated = true;
    }
  }

  if (!m_credentials.is_valid())
    throw std::invalid_argument{"Credentials not found"};

  if (m_token_json.is_null())
    throw std::invalid_argument{"Tokens not found"};

  auto botname = config.GetString(constants::KSCORD_SECTION, constants::BOT_CONFIG_KEY, "");

  if (botname.empty())
    log("No bot name provided");
  else
  {
    m_botname = botname;
    auto post_endpoint = config.GetString(botname, constants::POST_ENDPOINT_CONFIG_KEY, "");
    if (post_endpoint.empty())
      log("No post endpoint");
    else
      m_post_endpoint = post_endpoint;
  }
}

/**
 * FetchToken
 *
 * @returns [out] {bool}
 */
bool FetchToken(bool refresh = false) {
  using json = nlohmann::json;
  using namespace constants;
  // const std::string AUTHORIZATION_CODE_GRANT_TYPE{"authorization_code"};
  const std::string TOKEN_URL = BASE_URL + URLS.at(TOKEN_INDEX);

  auto body = (refresh) ?
    cpr::Body{
        "client_id=" + m_credentials.client_id + '&' +
        "client_secret=" + m_credentials.client_secret + '&' +
        "grant_type=refresh_token" + '&' +
        "refresh_token=" + m_auth.refresh_token + '&' +
        "redirect_uri=" + BASE_URL + URLS.at(OAUTH_INDEX) + '&' +
        "scope=" + m_credentials.scope
      } :
      cpr::Body{
        "client_id=" + m_credentials.client_id + '&' +
        "client_secret=" + m_credentials.client_secret + '&' +
        "grant_type=authorization_code" + '&' +
        "code=" + m_credentials.code + '&' +
        "redirect_uri=" + BASE_URL + URLS.at(OAUTH_INDEX) + '&' +
        "scope=" + m_credentials.scope
      };

  RequestResponse response{
    cpr::Post(
      cpr::Url{TOKEN_URL},
      cpr::Header{
        {"Content-Type", "application/x-www-form-urlencoded"}
      },
      body
    )
  };

  if (response.error) {
    kscord::log(response.GetError());
    return false;
  }

  json response_json = response.json();

  if (ValidateAuthJSON(response_json)) {
    m_auth = ParseAuthFromJSON(response_json);
    m_authenticated = true;
    m_token_json[m_username] = response_json;
    SaveToFile(m_token_json, m_tokens_path);
  }

  return true;
}

bool RefreshToken() {
  const bool REFRESH_TOKEN_REQUEST{true};
  return FetchToken(REFRESH_TOKEN_REQUEST);
}

bool FetchUser()
{
  using namespace constants;

  const std::string URL = BASE_URL + URLS.at(SELF_INDEX);

  if (m_auth.is_valid() || RefreshToken())
  {
    RequestResponse response{cpr::Get(
      cpr::Url{URL},
      cpr::Header{
        {"Content-Type", "application/json"},
        {HEADER_NAMES.at(HEADER_AUTH_INDEX), GetBearerAuth()}
     }
    )};

    if (response.error)
    {
      kscord::log(response.GetError());
      return false;
    }

    auto string = response.text();

    m_user = ParseUserFromJSON(response.json());
    return true;
  }
  return false;
}

User GetUser() {
  if (!m_user.is_valid())
    FetchUser();
  return m_user;
}


bool IsAuthenticated() {
  return m_authenticated;
}

bool HasValidToken() {
  return m_auth.is_valid();
}

void ClearToken() {
  m_auth = Auth{};
}

const std::string GetBearerAuth() {
  if (m_auth.access_token.empty())
    return "";
  return std::string{"Bearer " + m_auth.access_token};
}

const std::string GetBotAuth() const {
  std::string token{};

  if (!m_auth.bots.empty())
    if (m_botname.empty())
      token = m_auth.bots.front().token;
    else
    {
      auto it = std::find_if(m_auth.bots.cbegin(), m_auth.bots.cend(),
        [this](const BotInfo& bot)
        {
          return bot.name == m_botname;
        }
      );

      if (it != m_auth.bots.cend())
        token = it->token;
    }

  if (token.empty())
    throw std::runtime_error{"Bot auth not found"};

  return "Bot " + token;
}

const Credentials get_credentials() const {
  return m_credentials;
}

const std::string GetUsername() const {
  return m_username;
}

const std::string GetPostURL() const {
  if (m_post_endpoint.empty())
    throw std::runtime_error{"No post endpoint has been set"};
  return constants::BASE_URL + m_post_endpoint;
}

private:
using json = nlohmann::json;

Credentials  m_credentials;
Auth         m_auth;
User         m_user;
std::string  m_username;
std::string  m_botname;
bool         m_authenticated;
json         m_token_json;
json         m_credentials_json;
std::string  m_post_endpoint;
std::string  m_tokens_path;

};

} // namespace kscord
