#pragma once

#include <mach/mach.h>

#include <chrono>
#include <mutex>
#include <optional>
#include <vector>

namespace cheatengine {

class ValueMonitor {
public:
    struct MonitoredAddress {
        mach_vm_address_t address{0};
        std::size_t value_size{0};
        std::vector<std::uint8_t> last_value;
        std::chrono::steady_clock::time_point last_update{};
    };

    struct ValueChange {
        mach_vm_address_t address{0};
        std::vector<std::uint8_t> old_value;
        std::vector<std::uint8_t> new_value;
        std::chrono::steady_clock::time_point timestamp{};
    };

    void addAddress(mach_vm_address_t address, std::size_t size);
    void removeAddress(mach_vm_address_t address);
    std::vector<ValueChange> poll(task_t task);
    std::vector<MonitoredAddress> tracked() const;

private:
    std::vector<MonitoredAddress> addresses_;
    mutable std::mutex mutex_;
};

} // namespace cheatengine
