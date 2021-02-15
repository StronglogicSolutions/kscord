#include "kscord.test.hpp"


TEST(KStodonTests, InstantiateAuthenticator) {
  ASSERT_NO_THROW(kscord::Authenticator{});
}
