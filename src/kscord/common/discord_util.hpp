#pragma once

#include "types.hpp"
#include "util.hpp"

/**
  ┌───────────────────────────────────────────────────────────┐
  │░░░░░░░░░░░░░░░░░░░░░░░░░ HelperFns ░░░░░░░░░░░░░░░░░░░░░░░│
  └───────────────────────────────────────────────────────────┘
*/

// inline std::string GetConfigPath() {
//   return get_executable_cwd() + "../" + constants::DEFAULT_CONFIG_PATH;
// }

// inline INIReader GetConfigReader() {
//   return INIReader{GetConfigPath()};
// }

// inline bool SaveStatusID(uint64_t status_id, std::string username) {
//   using namespace nlohmann;
//   json database_json;
//   json loaded_json = LoadJSONFile(get_executable_cwd() + "../" + constants::DB_JSON_PATH);

//   if (loaded_json.is_discarded() || loaded_json.is_null())
//   {
//     database_json["status"] = json::object();
//     database_json["status"][username] = {status_id};
//   }
//   else
//   {
//     database_json = loaded_json;
//     json user_status_array_json = database_json["status"][username];

//     if (!user_status_array_json.empty()) {
//       for (const auto& id : user_status_array_json.get<const std::vector<uint64_t>>())
//         if (id == status_id) return false; // Already exists
//     }

//     database_json["status"][username].emplace_back(status_id);
//   }

//   SaveToFile(database_json, constants::DB_JSON_PATH);

//   return true;
// }

// inline std::vector<uint64_t> GetSavedStatusIDs(std::string username) {
//   using json = nlohmann::json;

//   json db_json = LoadJSONFile(get_executable_cwd() + "../" + constants::DB_JSON_PATH);

//   if (!db_json.is_null()                   &&
//       db_json.contains("status")           &&
//       db_json["status"].contains(username) &&
//       !db_json["status"][username].is_null()) {
//     return db_json["status"][username].get<std::vector<uint64_t>>();
//   }

//   return std::vector<uint64_t>{};
// }

// inline bool RemoveStatusID(std::string username, uint64_t id) {
//   using json = nlohmann::json;
//   bool result{false};
//   json db_json = LoadJSONFile(get_executable_cwd() + "../" + constants::DB_JSON_PATH);

//   if (!db_json.is_null()                 &&
//     db_json.contains("status")           &&
//     db_json["status"].contains(username) &&
//     !db_json["status"][username].is_null()) {

//     for (int i = 0; i < db_json["status"][username].size(); i++) {
//       if (db_json["status"][username][i].get<uint64_t>() == id) {
//         db_json["status"][username].erase(i);
//         result = true;
//         break;
//       }
//     }
//   }

//   if (result) {
//     SaveToFile(db_json, constants::DB_JSON_PATH);
//   }

//   return result;
// }

inline kscord::User ParseUserFromJSON(nlohmann::json data) {
  using namespace kjson;
  kscord::User user{};

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

  kscord::log(data.dump());

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
    channel.recipients = ParseUsersFromJSON(data["recipients"]);
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

// inline std::vector<Tag> ParseTagsFromJSON(nlohmann::json data) {
//   std::vector<Tag> tags_v{};

//   if (!data.is_null()) {
//     for (const auto& tag : data) {
//       tags_v.emplace_back(Tag{
//         .name = tag["name"],
//         .url  = tag["url"]
//       });
//     }
//   }

//   return tags_v;
// }

// inline std::vector<Mention> ParseMentionsFromJSON(nlohmann::json data) {
//   std::vector<Mention> mentions{};

//   if (!data.is_null()) {
//     for (const auto& mention : data) {
//       mentions.emplace_back(Mention{
//         .acct     = mention["acct"],
//         .id       = mention["id"],
//         .url      = mention["url"],
//         .username = mention["username"]
//       });
//     }
//   }

//   return mentions;
// }

// inline Media ParseMediaFromJSON(nlohmann::json data) {
//   Media media{};

//   if (!data.is_null() && data.is_object()) {
//     media.id                 = GetJSONStringValue(data, "id");
//     media.type               = GetJSONStringValue(data, "type");
//     media.url                = GetJSONStringValue(data, "url");
//     media.preview_url        = GetJSONStringValue(data, "preview_url");
//     media.remote_url         = GetJSONStringValue(data, "remote_url");
//     media.preview_remote_url = GetJSONStringValue(data, "preview_remote_url");
//     media.text_url           = GetJSONStringValue(data, "text_url");
//     media.description        = GetJSONStringValue(data, "description");
//     media.blurhash           = GetJSONStringValue(data, "blurhash");
//     media.meta               = MediaMetadata{};

//     if (data["meta"].contains("original")) {
//       auto original = data["meta"]["original"];

//       media.meta.original = MetaDetails{
//         .width    = GetJSONValue<uint32_t>(original, "width"),
//         .height   = GetJSONValue<uint32_t>(original, "height"),
//         .size     = GetJSONStringValue(original, "size"),
//         .aspect   = GetJSONValue<float>(original, "aspect")
//       };
//     }

//     if (data["meta"].contains("small")) {
//       auto small = data["meta"]["small"];

//       media.meta.small = MetaDetails{
//         .width    = small["width"],
//         .height   = small["height"],
//         .size     = small["size"],
//         .aspect   = small["aspect"]
//       };
//     }
//   }

//   return media;
// }

// inline std::vector<Media> ParseMediaFromJSONArr(nlohmann::json data) {
//   std::vector<Media> media_v{};

//   if (!data.is_null() && data.is_array()) {
//     for (const auto& item : data) {
//       media_v.emplace_back(ParseMediaFromJSON(item));
//     }
//   }

//   return media_v;
// }

// inline std::vector<User> ParseUsersFromJSON(nlohmann::json data) {
//   std::vector<User> users{};
//   std::string s = data.dump();

//   if (!data.is_null() && data.is_array()) {
//     for (const auto& item : data) {
//       if (!item.is_null() && item.is_object()) {
//         users.emplace_back(ParseUserFromJSON(item));
//       }
//     }
//   }
//   return users;
// }

// /**
//  * @brief
//  *
//  * @param data
//  * @return Status
//  */
// inline Status JSONToStatus(nlohmann::json data) {
//   Status status{};
//   std::string s = data.dump();
//   try {
//     if (!data.is_null() && data.is_object()) {
//     status.id                  = std::stoul(data["id"].get<std::string>());
//     status.created_at          = GetJSONStringValue    (data, "created_at");
//     status.replying_to_id      = GetJSONStringValue    (data, "in_reply_to_id");
//     status.replying_to_account = GetJSONStringValue    (data, "in_reply_to_account_id");
//     status.sensitive           = GetJSONBoolValue      (data, "sensitive");
//     status.spoiler             = GetJSONStringValue    (data, "spoiler_text");
//     status.visibility          = GetJSONStringValue    (data, "visibility");
//     status.language            = GetJSONStringValue    (data, "language");
//     status.uri                 = GetJSONStringValue    (data, "uri");
//     status.url                 = GetJSONStringValue    (data, "url");
//     status.replies             = GetJSONValue<uint32_t>(data, "replies_count");
//     status.reblogs             = GetJSONValue<uint32_t>(data, "reblogs_count");
//     status.favourites          = GetJSONValue<uint32_t>(data, "favourites_count");
//     status.content             = GetJSONStringValue    (data, "content");
//     status.reblog              = ParseAccountFromJSON  (data["reblog"]);;
//     status.application.name    = GetJSONStringValue    (data["application"], "name");
//     status.application.url     = GetJSONStringValue    (data["application"], "website");
//     status.account             = ParseAccountFromJSON  (data["account"]);
//     status.media               = ParseMediaFromJSONArr (data["media_attachments"]);
//     status.mentions            = ParseMentionsFromJSON (data["mentions"]);
//     status.tags                = ParseTagsFromJSON     (data["tags"]);
//     status.emojis              = GetJSONValue<std::vector<std::string>>(data, "emojis");
//   }

//   return status;
//   } catch (std::exception& e) {
//     std::cout << e.what();
//   }
//   return status;
// }

// inline std::vector<Status> JSONToStatuses(nlohmann::json data) {
//   std::vector<Status> statuses{};
//   if (!data.is_null() && data.is_array())
//     for (const auto& status_data : data)
//     {
//       Status status = JSONToStatus(status_data);
//       if (status.is_valid())
//         statuses.emplace_back(status);
//     }

//   return statuses;
// }


// inline std::vector<Status> JSONContextToStatuses(nlohmann::json data) {
//   std::vector<Status> statuses{};
//   std::string s = data.dump();

//   try {
//     for (const auto& context : data) {
//       auto new_statuses = JSONToStatuses(context);
//       statuses.insert(
//         statuses.end(),
//         std::make_move_iterator(new_statuses.begin()),
//         std::make_move_iterator(new_statuses.end())
//       );
//     }
//   }
//   catch (const std::exception& e) {
//     std::string error = e.what();
//     log(error);
//   }

//   return statuses;
// }
// inline std::vector<Conversation> JSONToConversation(nlohmann::json data) {
//   std::vector<Conversation> conversations{};

//   if (!data.is_null() && data.is_array()) {
//     for (const auto& item : data) {
//       if (!item.is_null() && item.is_object()) {
//         std::string s = item.dump();
//         conversations.emplace_back(
//           Conversation{
//             .id       = GetJSONStringValue(item, "id"),
//             .unread   = GetJSONBoolValue(item, "unread"),
//             .status   = JSONToStatus(item["last_status"]),
//             .accounts = ParseAccountsFromJSON(item["accounts"])
//           }
//         );
//       }
//     }
//   }
//   return conversations;
// }

// inline std::vector<Conversation> ParseRepliesFromConversations(std::vector<Conversation> conversations, std::vector<uint64_t> status_ids) {
//   std::vector<Conversation> replies{};

//   for (auto&& conversation : conversations) {
//     if (
//       std::find(
//         status_ids.cbegin(),
//         status_ids.cend(),
//         string_to_uint64(conversation.status.replying_to_id)) != status_ids.cend())
//       replies.emplace_back(std::move(conversation));
//   }

//   return replies;
// }

// inline std::string PlatformFromURL(const std::string& url) {
//   const std::string::size_type begin_length{8};
//   auto begin = url.find_first_of("https://");
//   std::string return_s{};
//   if (begin != std::string::npos) {
//   auto end = url.find_last_of('@');
//     if (end != std::string::npos) {
//       return_s = url.substr(begin + begin_length, (end - (begin + begin_length - 1)));
//     }
//   }
//   return return_s;
// }

// /**
//  * MakeMention
//  *
//  * TODO: Create full HTML mention:
//  * <span class=\"h-card\">
//  *   <a href=\"https://mastodon.online/@username\" class=\"u-url mention\">
//  *     @<span>username</span>
//  *   </a>
//  * </span>
//  *
//  * For example:
//  *
//  * "<span class=\"h-card\">"
//  * "  <a href=\"" + status.account.url + "/@" + status.account.username + "\" class=\"u-url mention\">"
//  * "    @" + status.account.username + '@' + PlatformFromURL(status.account.url) +
//  * "  </a>"
//  * "</span>"
//  *
//  * @param status
//  * @return std::string
//  */
// inline std::string MakeMention(const Status& status) {
//   if (!status.account.username.empty() && !status.account.url.empty()) {
//     return '@' + status.account.username + '@' + PlatformFromURL(status.account.url) + ' ';
//   }
//   return "";
// }
