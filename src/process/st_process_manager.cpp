#include "st_process_manager.h"

namespace Storytime {
    ProcessManager::~ProcessManager() {
        clear();
    }

    const std::list<Shared<Process>>& ProcessManager::get_processes() const {
        return processes;
    }

    void ProcessManager::add(const Shared<Process>& process) {
        processes.push_front(process);
    }

    void ProcessManager::remove(const Shared<Process>& process) {
        processes.remove(process);
    }

    void ProcessManager::remove_if(const std::function<bool(const Shared<Process>&)>& on_is_process_equal) {
        processes.remove_if(on_is_process_equal);
    }

    ProcessUpdateResult ProcessManager::update(const f64 timestep) {
        auto result = ProcessUpdateResult{
            .update_count = (u32) processes.size(),
            .success_count = 0,
            .fail_count = 0,
        };

        auto process_it = processes.begin();
        while (process_it != processes.end()) {

            Shared<Process> process = *process_it;
            ST_ASSERT_NOT_NULL(process);

            // Use the postfix operator (it++) instead of the prefix operator (++it) to copy the current process iterator
            // in case the current process needs to be removed from the list.
            auto current_process_it = process_it++;

            if (process->state == ProcessState::UNINITIALIZED) {
                process->state = ProcessState::RUNNING;
                process->on_initialize();
            }

            if (process->state == ProcessState::RUNNING) {
                process->on_update(timestep);
            }

            if (process->is_dead()) {
                if (process->state == ProcessState::SUCCEEDED) {
                    process->on_success();
                    Shared<Process> child = process->remove_child();
                    if (child != nullptr) {
                        add(child); // Start the child process when its parent process is complete.
                    } else {
                        result.success_count++; // Only count successes when the whole parent-child chain is completed
                    }
                } else if (process->state == ProcessState::FAILED) {
                    process->on_fail();
                    result.fail_count++;
                } else if (process->state == ProcessState::ABORTED) {
                    process->on_abort();
                    result.fail_count++;
                }
                processes.erase(current_process_it);
            }
        }

        return result;
    }

    void ProcessManager::abort(const bool abort_immediately) {
        auto process_it = processes.begin();
        while (process_it != processes.end()) {

            Shared<Process>& process = *process_it;
            ST_ASSERT_NOT_NULL(process);

            // Use the postfix operator (it++) instead of the prefix operator (++it) to copy the current process iterator
            // in case the current process needs to be removed from the list.
            auto current_process_it = process_it++;

            if (process->is_alive()) {
                process->set_state(ProcessState::ABORTED);
                if (abort_immediately) {
                    process->on_abort();
                    processes.erase(current_process_it);
                }
            }
        }
    }

    void ProcessManager::clear() {
        processes.clear();
    }

    bool ProcessManager::empty() const {
        return processes.empty();
    }
}
