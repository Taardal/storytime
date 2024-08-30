#include "resource_module.h"

namespace Storytime {
    ResourceModule::ResourceModule(SystemModule* system_module, AudioModule* audio_module)
        : system_module(system_module),
          resource_loader({
              .file_system = system_module->get<FileSystem>(),
              .audio_engine = audio_module->get<AudioEngine>()
          })
    {
        system_module->service_locator.set<ResourceLoader>(&resource_loader);
    }
}
