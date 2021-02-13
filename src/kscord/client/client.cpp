#include "client.hpp"

namespace kscord {

/**
  ┌───────────────────────────────────────────────────────────┐
  │░░░░░░░░░░░░░░░░░░░░░░░░░ Helper FNs ░░░░░░░░░░░░░░░░░░░░░░│
  └───────────────────────────────────────────────────────────┘
*/


/**
 * @brief Construct a new Client:: Client object
 *
 * @param username
 */
Client::Client(const std::string& username)
: m_authenticator(Authenticator{username}) {
  if (!m_authenticator.IsAuthenticated() && m_authenticator.RefreshToken())
    throw std::invalid_argument{"Client was unable to authenticate."};
}

bool Client::HasAuth() {
  return m_authenticator.IsAuthenticated();
}

User Client::FetchUser()
{
  return m_authenticator.GetUser();
}

Channel Client::FetchChannel(const std::string& id) {
  using namespace constants;

  const std::string URL = BASE_URL + URLS.at(CHANNELS_INDEX) + id;
  return Channel{};

}

std::vector<Channel> Client::FetchGuildChannels(const std::string& id)
{
  using namespace constants;

  const std::string URL = BASE_URL + URLS.at(GUILDS_INDEX) + id + "/channels";

  RequestResponse response{
    cpr::Get(
      cpr::Url(URL),
      cpr::Header{
        {"Content-Type", "application/json"},
        {HEADER_NAMES.at(HEADER_AUTH_INDEX), m_authenticator.GetBearerAuth()}
     }
    )
  };

  if (response.error)
  {
    log(response.GetError());
    return std::vector<Channel>{};
  }

  return ParseChannelsFromJSON(response.json());
}


Guild Client::FetchGuild(const std::string& id) {
  using namespace constants;

  const std::string URL = BASE_URL + URLS.at(GUILDS_INDEX) + id;

  RequestResponse response{
    cpr::Get(
      cpr::Url(URL),
      cpr::Header{
        {"Content-Type", "application/json"},
        {HEADER_NAMES.at(HEADER_AUTH_INDEX), m_authenticator.GetBearerAuth()}
     }
    )
  };

  if (response.error)
  {
    log(response.GetError());
    return Guild{};
  }

  return ParseGuildFromJSON(response.json());
}

std::vector<Guild> Client::FetchGuilds() {
  using namespace constants;

  const std::string URL = BASE_URL + URLS.at(SELF_GUILDS_INDEX);

  RequestResponse response{
    cpr::Get(
      cpr::Url(URL),
      cpr::Header{
        {"Content-Type", "application/json"},
        {HEADER_NAMES.at(HEADER_AUTH_INDEX), m_authenticator.GetBearerAuth()}
     }
    )
  };

  if (response.error)
  {
    log(response.GetError());
    return std::vector<Guild>{};
  }

  return ParseGuildsFromJSON(response.json());
}



} // namespace kstodon
