#include "client.hpp"

namespace kscord {

/**
  ┌───────────────────────────────────────────────────────────┐
  │░░░░░░░░░░░░░░░░░░░░░░░░░ Helper FNs ░░░░░░░░░░░░░░░░░░░░░░│
  └───────────────────────────────────────────────────────────┘
*/

/**
 * @brief Chunk Message
 *
 * @param   [in]  {std::string} message
 * @returns [out] {std::vector<std::string>}
 */
static std::vector<std::string> const ChunkMessage(const std::string& message) {
  using namespace constants;

  const uint32_t MAX_CHUNK_SIZE = DISCORD_CHAR_LIMIT - 6;

  std::vector<std::string>     chunks{};
  const std::string::size_type message_size = message.size();
  const std::string::size_type num_of_chunks{message.size() / MAX_CHUNK_SIZE + 1};
  uint32_t                     chunk_index{1};
  std::string::size_type       bytes_chunked{0};

  if (num_of_chunks > 1) {
    chunks.reserve(num_of_chunks);

    while (bytes_chunked < message_size) {
      const std::string::size_type size_to_chunk =
        (bytes_chunked + MAX_CHUNK_SIZE > message_size) ?
          (message_size - bytes_chunked) :
          MAX_CHUNK_SIZE;

      std::string oversized_chunk = message.substr(bytes_chunked, size_to_chunk);

      const std::string::size_type ws_idx = oversized_chunk.find_last_of(" ") + 1;
      const std::string::size_type pd_idx = oversized_chunk.find_last_of(".") + 1;
      const std::string::size_type index  =
        (size_to_chunk > MAX_CHUNK_SIZE) ?
          (ws_idx > pd_idx) ?
            ws_idx : pd_idx
            :
            size_to_chunk;

      chunks.emplace_back(
        index == 0 ?
        oversized_chunk :
        std::string{
          oversized_chunk.substr(0, index) + '\n' +
          std::to_string(chunk_index++)    + '/'  + std::to_string(num_of_chunks) // i/n
        }
      );

      bytes_chunked += index;
    }
  } else {
    chunks.emplace_back(message);
  }

  return chunks;
}

/**
 * @brief Construct a new Client:: Client object
 *
 * @param username
 */
Client::Client(bool fetch_new_token, const std::string& username)
: m_authenticator(Authenticator{username}) {
  if (fetch_new_token)
    m_authenticator.FetchToken();
  if (!m_authenticator.IsAuthenticated() && !m_authenticator.RefreshToken())
    log("Client was unable to authenticate.");
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
  cpr::Response response =
    cpr::Get(
      cpr::Url(URL),
      cpr::Header{
        {"Content-Type", "application/json"},
        {HEADER_NAMES.at(HEADER_AUTH_INDEX), m_authenticator.GetBotAuth()}
     }
    );

  if (response.error.code != cpr::ErrorCode::OK)
  {
    log(response.error.message);
    return Channel{};
  }

  return ParseChannelFromJSON(json::parse(response.text));
}

std::vector<Channel> Client::FetchGuildChannels(const std::string& id)
{
  using namespace constants;

  const std::string URL = BASE_URL + URLS.at(GUILDS_INDEX) + id + "/channels";

  cpr::Response response =
    cpr::Get(
      cpr::Url(URL),
      cpr::Header{
        {"Content-Type", "application/json"},
        {HEADER_NAMES.at(HEADER_AUTH_INDEX), m_authenticator.GetBotAuth()}}
    );

  if (response.error.code != cpr::ErrorCode::OK)
  {
    log(response.error.message);
    return std::vector<Channel>{};
  }

  return ParseChannelsFromJSON(json::parse(response.text));
}

Guild Client::FetchGuild(const std::string& id) {
  using namespace constants;

  const std::string URL = BASE_URL + URLS.at(GUILDS_INDEX) + id;

  cpr::Response response =
    cpr::Get(
      cpr::Url(URL),
      cpr::Header{
        {"Content-Type", "application/json"},
        {HEADER_NAMES.at(HEADER_AUTH_INDEX), m_authenticator.GetBotAuth()}
     });

  if (response.error.code != cpr::ErrorCode::OK)
  {
    log(response.error.message);
    return Guild{};
  }

  return ParseGuildFromJSON(json::parse(response.text));
}

std::vector<Guild> Client::FetchGuilds() {
  using namespace constants;

  const std::string URL = BASE_URL + URLS.at(SELF_GUILDS_INDEX);

  cpr::Response response =
    cpr::Get(
      cpr::Url(URL),
      cpr::Header{
        {"Content-Type", "application/json"},
        {HEADER_NAMES.at(HEADER_AUTH_INDEX), m_authenticator.GetBearerAuth()}
     });

  if (response.error.code != cpr::ErrorCode::OK)
  {
    log(response.error.message);
    return std::vector<Guild>{};
  }

  return ParseGuildsFromJSON(json::parse(response.text));
}

/**
 * @brief
 *
 * @param   [in]  {std::string}              content
 * @param   [in]  {std::vector<std::string>} urls
 * @returns [out] {bool}
 */
bool Client::PostMessage(const std::string& content, const std::vector<std::string>& urls)
{
  using namespace constants;
  bool           sending_media = !(urls.empty());
  nlohmann::json payload{};

  for (const auto& message_content : ChunkMessage(content))
  {
    payload["content"] = message_content;

    if (sending_media)
    {
      payload["embeds"] = nlohmann::json::array();
      for (const auto& url : urls)
        payload["embeds"].emplace_back(Embed{url}.to_json());
      sending_media = false;
    }

    auto s = payload.dump();

    log(s);

    cpr::Response response =
      cpr::Post(
        cpr::Url{m_authenticator.GetPostURL()},
        cpr::Header{
          {"Content-Type", "application/json"},
          // {HEADER_NAMES.at(HEADER_AUTH_INDEX), m_authenticator.GetBearerAuth()}
        },
        cpr::Body{
          payload.dump()
        });

    if (response.error.code != cpr::ErrorCode::OK)
    {
      log(response.error.message);
      return false;
    }

    log(response.text);

    payload.clear();
  }

  return true;
}

/**
 * @brief
 *
 * @returns [out] {std::string}
 */
std::string Client::FetchGateway()
{
  using namespace constants;

  const std::string URL = BASE_URL + URLS.at(GATEWAY_BOT_INDEX);

  cpr::Response response =
    cpr::Get(
      cpr::Url(URL),
      cpr::Header{
        {"Content-Type", "application/json"},
        {HEADER_NAMES.at(HEADER_AUTH_INDEX), m_authenticator.GetBotAuth()}
      });

  if (response.error.code != cpr::ErrorCode::OK)
  {
    log(response.error.message);
    return "";
  }

  /*
  {"url":
    "wss://gateway.discord.gg", "shards": 1, "session_start_limit": {
      "total": 1000, "remaining": 1000, "reset_after": 0, "max_concurrency": 1
    }
  }
  */

  return response.text;
}

bool Client::SetUser(const std::string& username)
{
  bool result = m_authenticator.SetUser(username);
  if (!result)
    m_last_error = "Failed to set user " + username;
  return result;
}

bool Client::CreateGatewaySocket(const std::string& url)
{
  zmq::context_t context{1};
  zmq::socket_t  socket{context, ZMQ_PULL};

  socket.connect(url);
  // socket.
  return false;
}

/**
 * @brief
 *
 * @return const std::string
 */
const std::string Client::GetUsername() const
{
  return m_authenticator.GetUsername();
}

/**
 * @brief
 *
 * @return const std::string
 */
const std::string Client::GetLastError() const
{
  return m_last_error;
}

} // namespace kscord
