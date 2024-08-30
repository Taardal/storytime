#include "resource_module.h"

namespace Storytime {
    ResourceModule::ResourceModule(SystemModule* system_module, AudioModule* audio_module)
        : resource_loader({
              .file_system = system_module->get<FileSystem>(),
              .audio_engine = audio_module->get<AudioEngine>()
          })
    {
        service_locator.set<ResourceLoader>(&resource_loader);
    }

    void ResourceModule::initialize() {
    }

    void ResourceModule::terminate() {
    }
}
