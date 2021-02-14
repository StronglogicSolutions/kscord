#pragma once

#include <cpr/cpr.h>
#include <kjson.hpp>

#include "constants.hpp"

namespace kscord {
class request_error : public std::logic_error {
public:
explicit request_error(const std::string message);
virtual ~request_error() override;

virtual const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW;

private:
std::string error_message;
};

std::string GetRequestError(cpr::ErrorCode code);

struct RequestResponse {
public:
bool          error;
cpr::Response response;

RequestResponse(cpr::Response r);

nlohmann::json json() const;

std::string text() const;

const std::string GetError() const;
};

} // namespace kscord
