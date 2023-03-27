#pragma once

#include "types.hpp"
#include "util.hpp"

namespace kscord {
/**
  ┌───────────────────────────────────────────────────────────┐
  │░░░░░░░░░░░░░░░░░░░░░░░░░ HelperFns ░░░░░░░░░░░░░░░░░░░░░░░│
  └───────────────────────────────────────────────────────────┘
*/

inline const std::string get_executable_cwd() {
  char* path = realpath("/proc/self/exe", NULL);
  char* name = basename(path);
  std::string executable_cwd{path, path + strlen(path) - strlen(name)};

  return executable_cwd;
}

inline const std::string GetConfigPath() {
  return get_executable_cwd() + "../" + constants::DEFAULT_CONFIG_PATH;
}

inline const std::string GetCredsJSONPath() {
  return get_executable_cwd() + "../" + constants::CREDS_JSON_PATH;
}

inline const std::string GetCredsJSON() {
  return LoadJSONFile(GetCredsJSONPath());
}

inline const std::string GetTokenJSON() {
  return LoadJSONFile(get_executable_cwd() + "../" + constants::TOKEN_JSON_PATH);
}

inline INIReader GetConfigReader() {
  return INIReader{GetConfigPath()};
}

inline kscord::User ParseUserFromJSON(nlohmann::json data)
{
  kscord::User user{};

  if (!data.is_null()) {
    user.id            = data["id"]           .get<std::string>();
    user.username      = data["username"]     .get<std::string>();
    user.discriminator = data["discriminator"].get<std::string>();
    user.avatar        = data["avatar"]       .get<std::string>();
    user.bot           = data["bot"]          .get<std::string>();
    user.system        = data["system"]       .get<std::string>();
    user.mfa_enabled   = data["mfa_enabled"]  .get<bool>();
    user.locale        = data["locale"]       .get<std::string>();
    user.verified      = data["verified"]     .get<bool>();
    user.email         = data["email"]        .get<std::string>();
    user.flags         = data["flags"]        .get<uint32_t>();
    user.premium_type  = data["premium_type"] .get<uint32_t>();
    user.public_flags  = data["public_flags"] .get<uint32_t>();
  }
  return user;
}

inline std::vector<kscord::User> ParseUsersFromJSON(const nlohmann::json& data)
{
  std::vector<kscord::User> users{};
  for (const auto& item : data)
    users.emplace_back(ParseUserFromJSON(data));
  return users;
}

inline kscord::Guild ParseGuildFromJSON(const nlohmann::json& data)
{
  kscord::Guild guild{};

  if (!data.is_null()) {
    guild.id                            = data["id"].get<std::string>();
    guild.name                          = data["name"].get<std::string>();
    guild.icon                          = data["icon"].get<std::string>();
    guild.icon_hash                     = data["icon_hash"].get<std::string>();
    guild.splash                        = data["splash"].get<std::string>();
    guild.discovery_splash              = data["discovery_splash"].get<std::string>();
    guild.owner                         = data["owner"].get<bool>();
    guild.owner_id                      = data["owner_id"].get<std::string>();
    guild.permissions                   = data["permissions"].get<uint32_t>();
    guild.region                        = data["region"].get<std::string>();
    guild.afk_channel_id                = data["afk_channel_id"].get<std::string>();
    guild.afk_timeout                   = data["afk_timeout"].get<uint32_t>();
    guild.widget_enabled                = data["widget_enabled"].get<bool>();
    guild.widget_channel_id             = data["widget_channel_id"].get<std::string>();
    guild.verification_level            = data["verification_level"].get<uint32_t>();
    guild.default_message_notifications = data["default_message_notifications"].get<uint32_t>();
    guild.explicit_content_filter       = data["explicit_content_filter"].get<uint32_t>();
  }

  return guild;
}

inline std::vector<kscord::Guild> ParseGuildsFromJSON(const nlohmann::json& data)
{
  std::vector<kscord::Guild> guilds{};

  if (data.is_array())
    for (const auto& item : data)
      guilds.emplace_back(ParseGuildFromJSON(item));

  return guilds;
}


inline kscord::Channel ParseChannelFromJSON(const nlohmann::json& data)
{
  kscord::Channel channel{};

  if (!data.is_null())
  {
    // channel.permission_overwrites = GetJSONValue<permission_overwrites(data, "]");
    // channel.recipients = ParseUsersFromJSON(data["recipients"]);
    channel.id                  = data["id"]                 .get<std::string>();
    channel.type                = data["type"]               .get<uint32_t>();
    channel.guild_id            = data["guild_id"]           .get<std::string>();
    channel.position            = data["position"]           .get<uint32_t>();
    channel.name                = data["name"]               .get<std::string>();
    channel.topic               = data["topic"]              .get<std::string>();
    channel.nsfw                = data["nsfw"]               .get<bool>();
    channel.last_message_id     = data["last_message_id"]    .get<std::string>();
    channel.bitrate             = data["bitrate"]            .get<uint32_t>();
    channel.user_limit          = data["user_limit"]         .get<uint32_t>();
    channel.rate_limit_per_user = data["rate_limit_per_user"].get<uint32_t>();
    channel.icon                = data["icon"]               .get<std::string>();
    channel.owner_id            = data["owner_id"]           .get<std::string>();
    channel.application_id      = data["application_id"]     .get<std::string>();
    channel.parent_id           = data["parent_id"]          .get<std::string>();
    channel.last_pin_timestamp  = data["last_pin_timestamp"] .get<std::string>();
  }

  return channel;
}

inline std::vector<kscord::Channel> ParseChannelsFromJSON(const nlohmann::json& data)
{
  std::vector<kscord::Channel> channels{};

  if (!data.is_null())
    for (const auto& item : data)
      channels.emplace_back(ParseChannelFromJSON(item));

  return channels;
}

} // namespace kscord
