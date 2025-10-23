#pragma once

#include <mach/kern_return.h>
#include <stdexcept>
#include <string>

namespace cheatengine {

class CheatEngineException : public std::runtime_error {
public:
    enum class ErrorType {
        PROCESS_ACCESS,
        MEMORY_OPERATION,
        SYSTEM_RESOURCE,
        INVALID_PARAMETER
    };

    CheatEngineException(ErrorType type, std::string message, int system_error = 0);

    [[nodiscard]] ErrorType type() const noexcept { return type_; }
    [[nodiscard]] int systemError() const noexcept { return system_error_; }

private:
    ErrorType type_;
    int system_error_;
};

std::string formatMachError(const char* call, kern_return_t code);

#define MACH_CHECK(call, error_type)                                                                \
    do {                                                                                            \
        kern_return_t kr__ = (call);                                                                \
        if (kr__ != KERN_SUCCESS) {                                                                 \
            throw ::cheatengine::CheatEngineException(                                              \
                (error_type),                                                                       \
                ::cheatengine::formatMachError(#call, kr__),                                        \
                static_cast<int>(kr__));                                                            \
        }                                                                                           \
    } while (0)

} // namespace cheatengine
