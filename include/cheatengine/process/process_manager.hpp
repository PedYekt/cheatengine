#pragma once

#include <mach/mach.h>

#include <optional>
#include <string>

namespace cheatengine {

class ProcessManager {
public:
    struct ProcessInfo {
        pid_t pid{0};
        std::string executable_path;
        task_t task_port{MACH_PORT_NULL};
        bool is_attached{false};
    };

    bool attach(pid_t pid);
    void detach();
    [[nodiscard]] std::optional<ProcessInfo> currentProcess() const noexcept;
    [[nodiscard]] bool ownsProcess(pid_t pid) const;

private:
    bool acquireTaskPort(pid_t pid, task_t& out_port);
    void resetState();

    ProcessInfo process_;
};

} // namespace cheatengine
