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
Client(const std::string& username = "");

virtual ~Client() override {}

virtual bool                HasAuth() override;


private:
using json = nlohmann::json;
Authenticator m_authenticator;
};

} // namespace kstodon
