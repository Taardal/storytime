#pragma once

#include "st_config.h"
#include "st_engine.h"
#include "st_app.h"

#define ST_MAIN(config, run_app) \
    try {\
        ::Storytime::main(config, run_app);\
    } catch (const ::Storytime::Error& e) {\
        ST_LOG_CRITICAL("Fatal error");\
        e.print_stacktrace();\
        return EXIT_FAILURE;\
    } catch (const std::exception& e) {\
        ST_LOG_CRITICAL("Fatal error [{}]: {}", ST_TYPE_NAME(e), e.what());\
        return EXIT_FAILURE;\
    }\
    return EXIT_SUCCESS

namespace Storytime {
    class Storytime {
    private:
        Config config;
        Engine* engine;

    public:
        Storytime(const Config& config, Engine* engine) : config(config), engine(engine) {
        }

        const Config& cfg() const {
            return config;
        }

        const CommandLineArguments& args() const {
            return config.command_line_arguments;
        }

        void run(App& app) const {
            engine->run(app);
        }

        void stop() const {
            engine->stop();
        }

        template<class T>
        T* get() const {
            return engine->get<T>();
        }
    };

    void main(const Config& config, const std::function<void(Storytime& storytime)>& on_run_app);
}