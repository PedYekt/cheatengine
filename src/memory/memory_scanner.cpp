#include "cheatengine/memory/memory_scanner.hpp"
#include "cheatengine/core/errors.hpp"

#include <algorithm>
#include <iterator>

#include <mach/mach_vm.h>

namespace cheatengine {

std::vector<MemoryRegion> MemoryScanner::enumerate(task_t task) const
{
    std::vector<MemoryRegion> regions;

    if (task == MACH_PORT_NULL){
        return regions;
    }

    mach_vm_address_t address = MACH_VM_MIN_ADDRESS;
    mach_vm_size_t size = 0;
    natural_t depth = 0;

    while(true){
        vm_region_submap_info_data_64_t info{};
        mach_msg_type_number_t info_count = VM_REGION_SUBMAP_INFO_COUNT_64;

        kern_return_t kr = mach_vm_region_recurse(
            task,
            &address,
            &size,
            &depth,
            reinterpret_cast<vm_region_recurse_info_t>(&info),
            &info_count);
        
        if (kr != KERN_SUCCESS){
            break;
        }

        if (info.is_submap){
            depth += 1;
            continue;
        }

        MemoryRegion region;
        region.start_address = address;
        region.size = size;
        region.protection = info.protection;
        region.is_shared = (info.share_mode != SM_PRIVATE);
        region.category = categorizeRegion(info, address);

        regions.push_back(region);

        address += size;
    }

    return regions;
}

std::vector<MemoryScanner::SearchResult> MemoryScanner::search(task_t task, const SearchValue& value) const
{
    std::vector<SearchResult> results;

    if (task == MACH_PORT_NULL) {
        return results;
    }

    const auto& needle = value.data();
    if (needle.empty()) {
        return results;
    }

    constexpr mach_vm_size_t chunk_size = 4096;
    constexpr std::size_t context_bytes = 16;

    const auto regions = enumerate(task);
    for (const auto& region : regions) {
        if (!region.flags().readable) {
            continue;
        }

        mach_vm_size_t offset = 0;
        while (offset < region.size) {
            mach_vm_size_t bytes_to_read =
                std::min(chunk_size, region.size - offset);

            std::vector<std::uint8_t> buffer;
            if (!readChunk(task, region.start_address + offset, bytes_to_read, buffer)) {
                offset += bytes_to_read;
                continue;
            }

            auto it = std::search(buffer.begin(), buffer.end(),
                                  needle.begin(), needle.end());

            while (it != buffer.end()) {
                const auto match_index =
                    static_cast<std::size_t>(std::distance(buffer.begin(), it));

                const mach_vm_address_t match_address =
                    region.start_address + offset + match_index;

                const std::size_t context_start =
                    (match_index > context_bytes)
                        ? match_index - context_bytes
                        : 0;

                const std::size_t context_end =
                    std::min(match_index + needle.size() + context_bytes,
                             buffer.size());

                using difference_type = std::vector<std::uint8_t>::difference_type;
                auto start_it = buffer.begin();
                std::advance(start_it, static_cast<difference_type>(context_start));
                auto end_it = buffer.begin();
                std::advance(end_it, static_cast<difference_type>(context_end));

                std::vector<std::uint8_t> context(start_it, end_it);

                SearchResult result;
                result.address = match_address;
                result.context = std::move(context);
                result.value_size = needle.size();
                results.push_back(std::move(result));

                it = std::search(it + 1, buffer.end(),
                                 needle.begin(), needle.end());
            }

            if (region.size - offset <= chunk_size) {
                break;
            }

            const mach_vm_size_t advance =
                chunk_size > needle.size()
                    ? chunk_size - static_cast<mach_vm_size_t>(needle.size() - 1)
                    : chunk_size;

            offset += advance;
        }
    }

    return results;
}

bool MemoryScanner::readChunk(task_t task,
    mach_vm_address_t address,
    std::size_t size,
    std::vector<std::uint8_t>& buffer) const
{
    if (task == MACH_PORT_NULL || size == 0) {
        buffer.clear();
        return false;
    }

    buffer.resize(size);

    mach_vm_size_t out_size = 0;

    kern_return_t kr = mach_vm_read_overwrite(
        task,
        address,
        static_cast<mach_vm_size_t>(size),
        reinterpret_cast<mach_vm_address_t>(buffer.data()),
        &out_size);

    if (kr != KERN_SUCCESS || out_size == 0) {
        buffer.clear();
        return false;
    }

    if (out_size < size) {
        buffer.resize(static_cast<std::size_t>(out_size));
    }

    return true;
}

} // namespace cheatengine
