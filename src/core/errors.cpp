#include "cheatengine/core/errors.hpp"

#include <mach/mach_error.h>
#include <sstream>



namespace cheatengine {

CheatEngineException::CheatEngineException(ErrorType type, std::string message, int system_error)
    : std::runtime_error(std::move(message))
    , type_(type)
    , system_error_(system_error)
{
}
std::string formatMachError(const char* call, kern_return_t code)
{
    const char* mach_message = mach_error_string(code);

    std::ostringstream oss;
    oss << call << " failed with error " << code;
    if (mach_message != nullptr) {
        oss << " (" << mach_message << ')';
    }
    return oss.str();
}

} // namespace cheatengine
