#pragma once

#include "kscord/common/auth/auth.hpp"
#include "kscord/common/interface.hpp"

namespace kscord {
/**
  @class
  ┌───────────────────────────────────────────────────────────┐
  │░░░░░░░░░░░░░░░░░░░░░░░░░░░ KSCord ░░░░░░░░░░░░░░░░░░░░░░░░│
  └───────────────────────────────────────────────────────────┘
*/

class Client : public SecureClient
{
public:
Client(bool fetch_new_token = false, const std::string& username = "");

virtual ~Client() override {}

virtual bool                HasAuth() override;


// TODO: declare interface for the following
User                FetchUser();
std::vector<Guild>  FetchGuilds();
Guild               FetchGuild(const std::string& id);
Channel             FetchChannel(const std::string& id);
std::vector<Channel>FetchGuildChannels(const std::string& id);

private:
using json = nlohmann::json;
Authenticator      m_authenticator;
std::vector<Guild> m_guilds;
};

} // namespace kscord
