#include "resource_module.h"

namespace Storytime {
    ResourceModule::ResourceModule(SystemModule* system_module, AudioModule* audio_module)
<<<<<<< HEAD
        : system_module(system_module),
          resource_loader({
              .file_system = system_module->get<FileSystem>(),
              .audio_engine = audio_module->get<AudioEngine>()
          })
=======
        : resource_loader({
            .file_system = &system_module->file_system,
            .audio_engine = &audio_module->audio_engine
        })
>>>>>>> 3071683 (Audio module)
    {
        system_module->service_locator.set<ResourceLoader>(&resource_loader);
    }
}
