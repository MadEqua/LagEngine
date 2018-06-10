#pragma once

#include <string>
#include <unordered_map>
#include <mutex>

#include "Types.h"
#include "Timer.h"
#include "IFrameListener.h"

namespace filewatch {
    template<class T>
    class FileWatch;
}

namespace Lag {
    class InitializationParameters;

    constexpr float TIME_TO_WAIT_FOR_RELOAD_S = 0.1f;
    constexpr float TIME_TO_DEBOUNCE_CALLBACK_S = 0.1f;

    class ResourceFilesWatcher : public IFrameListener {
    public:
        explicit ResourceFilesWatcher(const InitializationParameters &initializationParameters);
        ~ResourceFilesWatcher();

        void onFrameStart(float timePassed) override {}
        void onFrameRenderingQueued(float timePassed) override {}
        void onFrameEnd(float timePassed) override;

    private:

        enum class ReloadType : uint8 {
            IMAGE, MATERIAL, MESH, SHADER, RESOURCES_FILE, UNKNOWN
        };

        struct ReloadData {
            std::string dir, file;

            //We will wait a little bit before applying the reload.
            //It seems to be more stable and getting no resource file opening errors (especially when using symlinks).
            float timeLeftToExecute;
        };

        filewatch::FileWatch<std::string> *fileWatcher;
        Timer timer;

        std::mutex mutexMap;
        std::unordered_map<ReloadType, ReloadData> pendingReloads;

        void handleResourceChange(const InitializationParameters &initializationParameters, const std::string &path);
        void reloadMaterial(const ReloadData &reloadData) const;
        void reloadImage(const ReloadData &reloadData) const;
        void reloadShader(const ReloadData &reloadData) const;
        void reloadResourcesFile(const ReloadData &reloadData) const;
    };
}