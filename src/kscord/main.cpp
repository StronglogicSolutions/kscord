#include "config.hpp"

/**
 * kscord Main
 *
 * @param   [in]  {int}    argc
 * @param   [in]  {char**} argv
 * @returns [out] {int}
 */
int main(int argc, char** argv)
{
  kscord::Client client{};
  // kscord::Authenticator auth{"logicp"};
  // auth.FetchToken();

  client.PostMessage("Hi", {"https://pbs.twimg.com/profile_images/1399463161502240774/M8OkW5X3_400x400.jpg"});


  return 0;
}
