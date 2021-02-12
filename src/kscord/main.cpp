#include "config.hpp"

/**
 * kscord Main
 *
 * kscord Bot class will do one of the following:

 * 1. Post message to Mastodon
 * 2. Fetch replies to previous messages and send a new reply message
 *
 * @param   [in]  {int}    argc
 * @param   [in]  {char**} argv
 * @returns [out] {int}
 */
int main(int argc, char** argv)
{
  if (argc < 2)
    throw std::invalid_argument{"kscord called without arguments"};

  kscord::ExecuteConfig config {kscord::ParseRuntimeArguments(argc, argv)};

  kscord::Client client{};

  return 0;
}
