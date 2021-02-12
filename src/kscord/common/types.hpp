#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <cpr/cpr.h>
#include <kjson.hpp>

#include "util.hpp"

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

struct AccountField {
std::string name;
std::string value;
};

struct Account {
std::string               id;
std::string               username;
std::string               acct;
std::string               display_name;
bool                      locked;
bool                      bot;
bool                      discoverable;
std::string               group;
std::string               created_at;
std::string               note;
std::string               url;
std::string               avatar;
std::string               header;
std::string               header_static;
uint32_t                  followers_count;
uint32_t                  following_count;
uint32_t                  statuses_count;
std::string               last_status_at;
std::vector<AccountField> fields;

friend std::ostream &operator<<(std::ostream& o, const Account& a) {
  std::string fields{};
  for (const auto& field : a.fields) fields += "\nName: " + field.name + "\nValue: " + field.value;

  o << "ID:           " << a.id              << "\n" <<
       "Username:     " << a.username        << "\n" <<
       "Account:      " << a.acct            << "\n" <<
       "Display Name: " << a.display_name    << "\n" <<
       "Locked:       " << a.locked          << "\n" <<
       "Bot:          " << a.bot             << "\n" <<
       "Discoverable: " << a.discoverable    << "\n" <<
       "Group:        " << a.group           << "\n" <<
       "Created:      " << a.created_at      << "\n" <<
       "Note:         " << a.note            << "\n" <<
       "URL:          " << a.url             << "\n" <<
       "Avatar:       " << a.avatar          << "\n" <<
       "Header:       " << a.header          << "\n" <<
       "Followers:    " << a.followers_count << "\n" <<
       "Following:    " << a.following_count << "\n" <<
       "Statuses:     " << a.statuses_count  << "\n" <<
       "Last Status:  " << a.last_status_at  << "\n" <<

       "FIELDS\n"       << fields;

  return o;
}
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

struct Auth {
std::string access_token;
std::string token_type;
std::string scope;
std::string created_at;

bool is_valid() {
  return (
    !access_token.empty() &&
    !token_type.empty()   &&
    !scope.empty()        &&
    !created_at.empty()
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
