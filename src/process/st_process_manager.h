#pragma once

#include "st_process.h"

namespace Storytime {
    struct ProcessUpdateResult {
        u32 update_count = 0;
        u32 success_count = 0;
        u32 fail_count = 0;
    };

    class ProcessManager {
    private:
        std::list<Shared<Process>> processes{};

    public:
        ~ProcessManager();

        const std::list<Shared<Process>>& get_processes() const;

        void add(const Shared<Process>& process);

        void remove(const Shared<Process>& process);

        void remove_if(const std::function<bool(const Shared<Process>&)>& on_is_process_equal);

        ProcessUpdateResult update(f64 timestep);

        void abort(bool abort_immediately);

        void clear();

        bool empty() const;
    };
}

