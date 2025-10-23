#pragma once

#include "cheatengine/memory/memory_scanner.hpp"
#include "cheatengine/monitor/value_monitor.hpp"
#include "cheatengine/process/process_manager.hpp"
#include "cheatengine/writer/memory_writer.hpp"

namespace cheatengine {

class Application {
public:
    Application() = default;

    ProcessManager& processManager() { return process_manager_; }
    MemoryScanner& memoryScanner() { return memory_scanner_; }
    ValueMonitor& valueMonitor() { return value_monitor_; }
    MemoryWriter& memoryWriter() { return memory_writer_; }

private:
    ProcessManager process_manager_;
    MemoryScanner memory_scanner_;
    ValueMonitor value_monitor_;
    MemoryWriter memory_writer_;
};

} // namespace cheatengine
