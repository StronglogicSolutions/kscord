#pragma once

#include <string>
#include <vector>

namespace kscord {
namespace constants {
const std::string DEFAULT_CONFIG_PATH{"config/config.ini"};
const std::string CONFIG_JSON_PATH   {"config/credentials.json"};
const std::string TOKEN_JSON_PATH    {"config/tokens.json"};
const std::string KSCORD_SECTION{"kscord"};
const std::string USER_CONFIG_KEY{"user"};

const uint8_t  HEADER_AUTH_INDEX   = 0x00;
const std::vector<std::string> HEADER_NAMES{
"Authorization"
};

const std::string BASE_URL{"https://discord.com"};

const uint8_t OAUTH_INDEX      {0x00};
const uint8_t TOKEN_INDEX      {0x01};
const uint8_t SELF_INDEX       {0x02};
const uint8_t SELF_GUILDS_INDEX{0x03};
const uint8_t CHANNELS_INDEX{0x04};
const uint8_t GUILDS_INDEX{0x05};

const std::vector<std::string> URLS{
  "/api/oauth2/authorize",
  "/api/oauth2/token",
  "/api/users/@me",
  "/api/users/@me/guilds",
  "/api/channels/",
  "/api/guilds/"
};


const uint8_t BOT_INDEX{0x00};
const uint8_t CONNECTIONS_INDEX{0x01};
const uint8_t EMAIL_INDEX{0x02};
const uint8_t IDENTIFY_INDEX{0x03};
const uint8_t GUILDS_SCOPE_INDEX{0x04};
const uint8_t GUILDS_JOIN_INDEX{0x05};
const uint8_t GDM_JOIN_INDEX{0x06};
// const uint8_t MESSAGES_REA_INDEX{0x01};
// const uint8_t RPC_INDEX{0x01};
// const uint8_t RPC_API_INDEX{0x01};
// const uint8_t RPC_NOTIFICATIONS_READ_INDEX{0x01};
// const uint8_t WEBHOOK_INCOMING_INDEX{0x01};
// const uint8_t APPLICATIONS_BUILDS_UPLOAD_INDEX{0x01};
// const uint8_t APPLICATIONS_BUILDS_READ_INDEX{0x01};
// const uint8_t APPLICATIONS_STORE_UPDATE_INDEX{0x01};
// const uint8_t APPLICATIONS_ENTITLEMENTS_INDEX{0x01};
// const uint8_t RELATIONSHIPS_READ_INDEX{0x01};
// const uint8_t ACTIVITIES_READ_INDEX{0x01};
// const uint8_t ACTIVITIES_WRITE_INDEX{0x01};
// const uint8_t APPLICATIONS_COMMANDS_INDEX{0x01};
// const uint8_t APPLICATIONS_COMMANDS_UPDATE_INDEX{0x01};

const std::vector<std::string> SCOPES{
 "bot",
 "connections",
 "email",
 "identify",
 "guilds",
 "guilds.join",
 "gdm.join",
 "messages.rea",
 "rpc",
 "rpc.api",
 "rpc.notifications.read",
 "webhook.incoming",
 "applications.builds.upload",
 "applications.builds.read",
 "applications.store.update",
 "applications.entitlements	",
 "relationships.read",
 "activities.read	",
 "activities.write",
 "applications.commands",
 "applications.commands.update"
};
} // namespace constants
} // namespace kscord
