#include "resource_module.h"

namespace Storytime {
    ResourceModule::ResourceModule(SystemModule* system_module, AudioModule* audio_module)
        : resource_loader({
            .file_system = &system_module->file_system,
            .audio_engine = &audio_module->audio_engine
        })
    {
        system_module->service_locator.set<ResourceLoader>(&resource_loader);
    }
}
