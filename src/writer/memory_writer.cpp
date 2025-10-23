#include "cheatengine/writer/memory_writer.hpp"

namespace cheatengine {

bool MemoryWriter::write(task_t, mach_vm_address_t address, const std::vector<std::uint8_t>& data)
{
    WriteOperation op;
    op.address = address;
    op.new_value = data;
    op.timestamp = std::chrono::steady_clock::now();
    op.success = false;
    recordOperation(std::move(op));
    return false;
}

bool MemoryWriter::canWrite(task_t, mach_vm_address_t, std::size_t) const
{
    return false;
}

std::vector<MemoryWriter::WriteOperation> MemoryWriter::history() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return history_;
}

void MemoryWriter::recordOperation(WriteOperation operation)
{
    std::lock_guard<std::mutex> lock(mutex_);
    history_.push_back(std::move(operation));
}

} // namespace cheatengine
