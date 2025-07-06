#include "st_process.h"

namespace Storytime {
    ProcessState Process::get_state() const {
        return state;
    }

    void Process::set_state(const ProcessState state) {
        this->state = state;
    }

    Shared<Process> Process::get_child() const {
        return child;
    }

    void Process::set_child(const Shared<Process>& child) {
        ST_ASSERT(child != nullptr, "Child process cannot be null");
        this->child = child;
    }

    Shared<Process> Process::remove_child() {
        if (child == nullptr) {
            return nullptr;
        }
        Shared<Process> removed_child = child;
        child.reset();
        return removed_child;
    }

    void Process::succeed() {
        ST_ASSERT(
            state == ProcessState::RUNNING || state == ProcessState::PAUSED,
            "Cannot succeed a process that is not already running or paused"
        );
        state = ProcessState::SUCCEEDED;
    }

    void Process::fail() {
        ST_ASSERT(
            state == ProcessState::RUNNING || state == ProcessState::PAUSED,
            "Cannot fail a process that is not already running or paused"
        );
        state = ProcessState::FAILED;
    }

    void Process::abort() {
        state = ProcessState::ABORTED;
    }

    void Process::pause() {
        ST_ASSERT(state == ProcessState::RUNNING, "Cannot pause a process that isn't running");
        state = ProcessState::PAUSED;
    }

    void Process::resume() {
        ST_ASSERT(state == ProcessState::PAUSED, "Cannot resume a process that isn't paused");
        state = ProcessState::RUNNING;
    }

    bool Process::is_alive() const {
        return state == ProcessState::RUNNING || state == ProcessState::PAUSED;
    }

    bool Process::is_dead() const {
        return state == ProcessState::SUCCEEDED || state == ProcessState::FAILED || state == ProcessState::ABORTED;
    }

    bool Process::is_paused() const {
        return state == ProcessState::PAUSED;
    }
}
