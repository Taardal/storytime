#pragma once

namespace Storytime {
    enum class ProcessState {
        UNINITIALIZED = 0,  // Created but not running
        RUNNING = 1,        // Initialized and running
        PAUSED = 2,         // Initialized but paused
        SUCCEEDED = 3,      // Completed successfully
        FAILED = 4,         // Failed to complete
        ABORTED = 5,        // Aborted; may not have started
    };

    class Process {
        friend class ProcessManager;

    private:
        ProcessState state = ProcessState::UNINITIALIZED;
        Shared<Process> child = nullptr;

    protected:
        virtual void on_initialize() {}

        virtual void on_update(f64 timestep) = 0;

        virtual void on_success() {}

        virtual void on_fail() {}

        virtual void on_abort() {}

    public:
        virtual ~Process() = default;

        ProcessState get_state() const;

        void set_state(ProcessState state);

        Shared<Process> get_child() const;

        void set_child(const Shared<Process>& child);

        Shared<Process> remove_child();

        void succeed();

        void fail();

        void abort();

        void pause();

        void resume();

        bool is_alive() const;

        bool is_dead() const;

        bool is_paused() const;
    };

}