#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <cpr/cpr.h>
#include <kjson.hpp>

namespace kscord {
/**
  ┌───────────────────────────────────────────────────────────┐
  │░░░░░░░░░░░░░░░░░░░░░░░░░ CONSTANTS ░░░░░░░░░░░░░░░░░░░░░░│
  └───────────────────────────────────────────────────────────┘
*/
namespace constants {

};


/**
  ┌───────────────────────────────────────────────────────────┐
  │░░░░░░░░░░░░░░░░░░░░░░░░░░ STRUCTS ░░░░░░░░░░░░░░░░░░░░░░░│
  └───────────────────────────────────────────────────────────┘
*/


struct User {
std::string id;
std::string username;
std::string discriminator;
std::string avatar;
std::string bot;
std::string system;
std::string mfa_enabled;
std::string locale;
std::string verified;
std::string email;
std::string flags;
std::string premium_type;
std::string public_flags;

bool is_valid()
{
  return (
    !id.empty()       &&
    !username.empty()
  );
}

friend std::ostream &operator<<(std::ostream& o, const User& u) {
  o << "ID: " + u.id + '\n' +
       "Username: " + u.username +'\n' +
       "Discriminator: " + u.discriminator +'\n' +
       "Avatar: " + u.avatar +'\n' +
       "Bot: " + u.bot +'\n' +
       "System: " + u.system +'\n' +
       "MFA_enabled: " + u.mfa_enabled +'\n' +
       "Locale: " + u.locale +'\n' +
       "Verified: " + u.verified +'\n' +
       "Email: " + u.email +'\n' +
       "Flags: " + u.flags +'\n' +
       "Premium_type: " + u.premium_type +'\n' +
       "Public_flags: " + u.public_flags;
  return o;
}
};

struct EmbedImage
{
std::string url;
// proxy_url
uint32_t height;
uint32_t width;

const nlohmann::json to_json()
{
  nlohmann::json json{};
  json["url"] = url;

  return json;
}
};

struct EmbedVideo
{
std::string url;
// proxy_url
uint32_t height;
uint32_t width;
};

struct EmbedThumbnail
{
std::string url;
// proxy_url
uint32_t height;
uint32_t width;
};

struct EmbedFooter
{
std::string text;
std::string icon_url;
// proxy_icon_url
};

struct EmbedAuthor{
std::string name;
std::string url;
std::string icon_url;
// proxy_icon_url
};

struct EmbedField
{
std::string name;
std::string value;
bool is_inline;
};

struct Attachment
{
std::string id;
std::string filename;
uint32_t size;
std::string url;
// proxy_url
uint32_t height;
uint32_t width;
};

struct EmbedProvider
{
std::string name;
std::string url;
};

struct Embed
{
Embed(const std::string& image_url)
: image{EmbedImage{.url = image_url}},
  url{image_url} {}

std::string title;
std::string type;
std::string description;
std::string url;
std::string timestamp;
uint32_t color;
EmbedFooter	 footer;
EmbedImage	 image;
EmbedThumbnail thumbnail;
EmbedVideo video;
EmbedProvider provider;
EmbedAuthor	author;
std::vector<EmbedField> fields;

const nlohmann::json to_json()
{
  nlohmann::json json{};
  json["title"] = title;
  json["description"] = description;
  json["url"] = url;
  json["image"] = image.to_json();

  return json;
}
};

struct Message {
std::string id;
std::string channel_id;
std::string guild_id;
User        author;
// std::string member guild member;
std::string content;
std::string timestamp;
std::string edited_timestamp;
std::string tts;
std::string mention_everyone;
std::vector<User> mentions;
// std::string mention_roles
// std::string mention_channels
// std::string attachments // TODO: important
// std::string embeds // TODO: also
// std::string reactions
// std::string nonce
// std::string pinned
// std::string webhook_id
// std::string type
// std::string activity
// std::string application
// std::string message_reference
// std::string flags
// std::string stickers
// std::string referenced_message
};

struct Channel {
std::string       id;
uint32_t          type;
std::string       guild_id;
uint32_t          position;
//                permission_overwrites TODO: [ Overwrite objects            ];
std::string       name;
std::string       topic;
bool              nsfw;
std::string       last_message_id;
uint32_t          bitrate;
uint32_t          user_limit;
uint32_t          rate_limit_per_user;
std::vector<User> recipients;
std::string       icon;
std::string       owner_id;
std::string       application_id;
std::string       parent_id;
std::string       last_pin_timestamp;
};

struct Guild {
std::string              id;
std::string              name;
std::string              icon;
std::string              icon_hash;
std::string              splash;
std::string              discovery_splash;
bool                     owner;
std::string              owner_id;
uint32_t                 permissions;
std::string              region;
std::string              afk_channel_id;
uint32_t                 afk_timeout;
bool                     widget_enabled;
std::string              widget_channel_id;
uint32_t                 verification_level;
uint32_t                 default_message_notifications;
uint32_t                 explicit_content_filter;
// roles
// emojis
std::vector<std::string> features;
std::string              permissions_new;
// mfa_level
// application_id
// system_channel_id
// system_channel_flags
// rules_channel_id
// joined_at
// large
// unavailable
// member_count
// voice_states
// members
std::vector<Channel> channels;
// presences
// max_presences
// max_members
// vanity_url_code
// description
// banner
// premium_tier
// premium_subscription_count
// preferred_locale
// public_updates_channel_id
// max_video_channel_users
// approximate_member_count
// approximate_presence_count
// welcome_screen
};

struct Mention {
std::string acct;
std::string id;
std::string url;
std::string username;

friend std::ostream &operator<<(std::ostream& o, const Mention& m) {
  o << "ID: " << m.id << "\n" << "Acct: " << m.acct << "\n" << "URL: " << m.url << "\nUsername:" << m.username << std::endl;

  return o;
}

};


struct Credentials {
std::string scope;
std::string client_id;
std::string client_secret;
std::string public_key;
std::string code;

bool is_valid() {
  return
    !scope.empty() &&
    !client_id.empty() &&
    !client_secret.empty() &&
    !public_key.empty() &&
    !code.empty();
}
};

struct BotInfo {
std::string name;
std::string token;
};

struct Auth {
std::string access_token;
std::string refresh_token;
std::string token_type;
std::string scope;
std::string expires_in;
std::vector<BotInfo> bots;

bool is_valid() {
  return (
    !access_token.empty() &&
    !token_type.empty()   &&
    !scope.empty()        &&
    !expires_in.empty()
  );
}
};

struct Application {
std::string name;
std::string url;

friend std::ostream &operator<<(std::ostream& o, const Application& a) {
  o << "Name: " << a.name << "\n" << "URL: " << a.url << std::endl;

  return o;
}
};

} // namespace kscord
