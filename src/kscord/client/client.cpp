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
Client::Client(bool fetch_new_token, const std::string& username)
: m_authenticator(Authenticator{username}) {
  if (fetch_new_token)
    m_authenticator.FetchToken();
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
        {HEADER_NAMES.at(HEADER_AUTH_INDEX), m_authenticator.GetBotAuth()}
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
        {HEADER_NAMES.at(HEADER_AUTH_INDEX), m_authenticator.GetBotAuth()}
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

bool Client::PostMessage(const std::string& content)
{
  using namespace constants;

  nlohmann::json payload{};
  payload["content"] = content;
  RequestResponse response{
    cpr::Post(
      cpr::Url{m_authenticator.GetPostURL()},
      cpr::Header{
        {"Content-Type", "application/json"},
        // {HEADER_NAMES.at(HEADER_AUTH_INDEX), m_authenticator.GetBearerAuth()}
      },
      cpr::Body{
        payload.dump()
      }
    )
  };

  if (response.error)
  {
    log(response.GetError());
    return false;
  }

  log(response.text());

  return true;

}

std::string Client::FetchGateway()
{
  using namespace constants;

  const std::string URL = BASE_URL + URLS.at(GATEWAY_BOT_INDEX);

  RequestResponse response{
    cpr::Get(
      cpr::Url(URL),
      cpr::Header{
        {"Content-Type", "application/json"},
        {HEADER_NAMES.at(HEADER_AUTH_INDEX), m_authenticator.GetBotAuth()}
      }
    )
  };

  if (response.error)
  {
    log(response.GetError());
    return "";
  }

  /*
  {"url":
    "wss://gateway.discord.gg", "shards": 1, "session_start_limit": {
      "total": 1000, "remaining": 1000, "reset_after": 0, "max_concurrency": 1
    }
  }
  */

  return response.text();
}


bool Client::CreateGatewaySocket(const std::string& url)
{
  zmq::context_t context{1};
  zmq::socket_t  socket{context, ZMQ_PULL};

  socket.connect(url);
  // socket.

  return false;
}


} // namespace kscord
