
#include "utils/status.h"
#include <sstream>

namespace utils {

namespace {
std::string ErrorCodeToString(Status::Code error_code) {
  switch (error_code) {
      // Not an error; returned on success
  case Status::Success:
      return "No error";
  case Status::Unknown:
      return "Unknown error";
          // The operation was cancelled (typically by the caller).
  case Status::Cancelled:
      return "The operation was cancelled";
  case Status::InternalError:
      return "Internal errors";
  case Status::InvalidArgument:
      return "Client specified an invalid argument";
  case Status::InvalidUrl:
      return "Specified Invalid url";
  case Status::HttpError:
      return "Http error received";
  case Status::ConnectionError:
      return "Server connection error";
    
  case Status::BufferOverflow:
      return "Internal buffer overflow";
 
  case Status::Timeout:
      return "The operation timed out";
  case Status::NoMemory:
      return "No memory";
  case Status::FileFailure:
      return "Cannot open/read or write to file";
  case Status::EndOfFile:
      return "End of file";
    default:
      return "Unknown status";
  }
}
}  // namespace


const Status Status::SUCCESS = Status(Status::Success, "");
const Status Status::UNKNOWN = Status(Status::Unknown, "");

std::string Status::ToString() const {
  if (error_code_ == Status::Success)
    return "OK";

  std::stringstream ss;

  ss << error_code() << " (" << ErrorCodeToString(error_code_) << "): " << error_message_ << std::endl;
  return ss.str();
  /*return base::StringPrintf("%d (%s): %s",
                            error_code_,
                            error::ErrorCodeToString(error_code_).c_str(),
                            error_message_.c_str());*/
}

std::ostream& operator<<(std::ostream& os, const Status& x) {
  os << x.ToString();
  return os;
}

} 

