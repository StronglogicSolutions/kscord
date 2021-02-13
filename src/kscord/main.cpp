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

  client.PostMessage("Whatsup");


  return 0;
}
