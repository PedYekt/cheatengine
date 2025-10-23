#pragma once

#include "cheatengine/memory/memory_region.hpp"
#include "cheatengine/memory/value_types.hpp"

#include <mach/mach.h>

#include <vector>

namespace cheatengine {

class MemoryScanner {
public:
    struct SearchResult {
        mach_vm_address_t address{0};
        std::vector<std::uint8_t> context;
        std::size_t value_size{0};
    };

    std::vector<MemoryRegion> enumerate(task_t task) const;
    std::vector<SearchResult> search(task_t task, const SearchValue& value) const;
    bool readChunk(task_t task, mach_vm_address_t address, std::size_t size, std::vector<std::uint8_t>& buffer) const;
};

} // namespace cheatengine
