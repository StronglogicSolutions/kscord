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
  if (!m_authenticator.HasValidToken()) {
    m_authenticator.FetchToken();
  }

  if (!m_authenticator.HasValidToken())
    throw std::invalid_argument{"Client was unable to authenticate."};
}

bool Client::HasAuth() {
  return m_authenticator.IsAuthenticated();
}

} // namespace kstodon
