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

        void add(const Shared<Process>& process);

        ProcessUpdateResult update(f64 timestep);

        void abort(bool abort_immediately);

        void clear();

        bool empty() const;
    };
}

