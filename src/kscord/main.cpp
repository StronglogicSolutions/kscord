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

  // auto guilds = client.FetchGuilds();

  // auto num = guilds.size();

  // kscord::log(std::to_string(num));

  for (const kscord::Guild& guild : client.FetchGuilds())
  {
    if (guild.name == "KSTYLEYO")
    {
      auto my_guild = client.FetchGuild(guild.id);
      std::vector<kscord::Channel> channels = client.FetchGuildChannels(guild.id);
      if (!channels.empty()) {
        kscord::log(std::to_string(channels.size()));
      }
    }
  }


  return 0;
}
