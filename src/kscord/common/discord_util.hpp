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

inline kscord::User ParseUserFromJSON(nlohmann::json data) {
  kscord::User user{};
  using namespace kjson;

  if (!data.is_null()) {
    user.id = GetJSONStringValue(data, "id");
    user.username = GetJSONStringValue(data, "username");
    user.discriminator = GetJSONStringValue(data, "discriminator");
    user.avatar = GetJSONStringValue(data, "avatar");
    user.bot = GetJSONStringValue(data, "bot");
    user.system = GetJSONStringValue(data, "system");
    user.mfa_enabled = GetJSONBoolValue(data, "mfa_enabled");
    user.locale = GetJSONStringValue(data, "locale");
    user.verified = GetJSONBoolValue(data, "verified");
    user.email = GetJSONStringValue(data, "email");
    user.flags = GetJSONValue<uint32_t>(data, "flags");
    user.premium_type = GetJSONValue<uint32_t>(data, "premium_type");
    user.public_flags = GetJSONValue<uint32_t>(data, "public_flags");
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

inline kscord::Guild ParseGuildFromJSON(const nlohmann::json& data) {
  using namespace kjson;

  kscord::Guild guild{};

  if (!data.is_null()) {
    guild.id = GetJSONStringValue(data, "id");
    guild.name = GetJSONStringValue(data, "name");
    guild.icon = GetJSONStringValue(data, "icon");
    guild.icon_hash = GetJSONStringValue(data, "icon_hash");
    guild.splash = GetJSONStringValue(data, "splash");
    guild.discovery_splash = GetJSONStringValue(data, "discovery_splash");
    guild.owner = GetJSONBoolValue(data, "owner");
    guild.owner_id = GetJSONStringValue(data, "owner_id");
    guild.permissions = GetJSONValue<uint32_t>(data, "permissions");
    guild.region = GetJSONStringValue(data, "region");
    guild.afk_channel_id = GetJSONStringValue(data, "afk_channel_id");
    guild.afk_timeout = GetJSONValue<uint32_t>(data, "afk_timeout");
    guild.widget_enabled = GetJSONBoolValue(data, "widget_enabled");
    guild.widget_channel_id = GetJSONStringValue(data, "widget_channel_id");
    guild.verification_level = GetJSONValue<uint32_t>(data, "verification_level");
    guild.default_message_notifications = GetJSONValue<uint32_t>(data, "default_message_notifications");
    guild.explicit_content_filter = GetJSONValue<uint32_t>(data, "explicit_content_filter");
  }

  return guild;
}

inline std::vector<kscord::Guild> ParseGuildsFromJSON(const nlohmann::json& data) {
  std::vector<kscord::Guild> guilds{};

  if (data.is_array())
    for (const auto& item : data)
      guilds.emplace_back(ParseGuildFromJSON(item));

  return guilds;
}


inline kscord::Channel ParseChannelFromJSON(const nlohmann::json& data) {
  using namespace kjson;

  kscord::Channel channel{};

  if (!data.is_null()) {
    channel.id = GetJSONStringValue(data, "id");
    channel.type = GetJSONValue<uint32_t>(data, "type");
    channel.guild_id = GetJSONStringValue(data, "guild_id");
    channel.position = GetJSONValue<uint32_t>(data, "position");
    // channel.permission_overwrites = GetJSONValue<permission_overwrites(data, "]");
    channel.name = GetJSONStringValue(data, "name");
    channel.topic = GetJSONStringValue(data, "topic");
    channel.nsfw = GetJSONBoolValue(data, "nsfw");
    channel.last_message_id = GetJSONStringValue(data, "last_message_id");
    channel.bitrate = GetJSONValue<uint32_t>(data, "bitrate");
    channel.user_limit = GetJSONValue<uint32_t>(data, "user_limit");
    channel.rate_limit_per_user = GetJSONValue<uint32_t>(data, "rate_limit_per_user");
    // channel.recipients = ParseUsersFromJSON(data["recipients"]);
    channel.icon = GetJSONStringValue(data, "icon");
    channel.owner_id = GetJSONStringValue(data, "owner_id");
    channel.application_id = GetJSONStringValue(data, "application_id");
    channel.parent_id = GetJSONStringValue(data, "parent_id");
    channel.last_pin_timestamp = GetJSONStringValue(data, "last_pin_timestamp");
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
