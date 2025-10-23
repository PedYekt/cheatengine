#include "cheatengine/process/process_manager.hpp"

#include <libproc.h>        // proc_pidinfo / proc_pidpath
#include <sys/proc_info.h>   // PROC_PIDTBSDINFO constants
#include <unistd.h>          // getuid
#include <mach/mach_init.h>  // mach_task_self
#include <mach/task.h>       // task_for_pid


namespace cheatengine {

bool ProcessManager::attach(pid_t pid)
{
    if (process_.is_attached){
        detach();
    }

    if (!ownsProcess(pid)){
        return false;
    }

    task_t task = MACH_PORT_NULL;
    if (!acquireTaskPort(pid, task)){
        return false;
    }

    ProcessInfo info;
    info.pid = pid;
    info.task_port = task;
    info.is_attached = true;

    char path_buffer[PROC_PIDPATHINFO_MAXSIZE] = {};
    const int path_length = proc_pidpath(pid, path_buffer, sizeof(path_buffer));

    if (path_length > 0) {
        info.executable_path.assign(path_buffer, static_cast<size_t>(path_length));
    }

    process_ = std::move(info);
    return true;
}

void ProcessManager::detach()
{
    resetState();
}

std::optional<ProcessManager::ProcessInfo> ProcessManager::currentProcess() const noexcept
{
    if (process_.is_attached) {
        return process_;
    }
    return std::nullopt;
}

bool ProcessManager::ownsProcess(pid_t pid) const
{
    if (pid <= 0) {
        return false; // invalid PID
    }

    struct proc_bsdinfo info {};
    const int result =
        proc_pidinfo(pid, PROC_PIDTBSDINFO, 0, &info, PROC_PIDTBSDINFO_SIZE);

    if (result != PROC_PIDTBSDINFO_SIZE) {
        return false; 
    }

    const uid_t current_uid = getuid();
    return info.pbi_uid == current_uid;
}

bool ProcessManager::acquireTaskPort(pid_t pid, task_t& out_port)
{
    out_port = MACH_PORT_NULL;

    const kern_return_t kr = task_for_pid(mach_task_self(), pid, &out_port);

    if (kr != KERN_SUCCESS){
        out_port = MACH_PORT_NULL;
        return false;
    }
                
    return true;
}

void ProcessManager::resetState()
{
    if (process_.task_port != MACH_PORT_NULL) {
        mach_port_deallocate(mach_task_self(), process_.task_port);
    }
    process_ = {};
}

} // namespace cheatengine