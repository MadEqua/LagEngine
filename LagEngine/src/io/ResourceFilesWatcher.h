#pragma once

#include <string>
#include <unordered_set>
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
            ReloadType type;
            std::string dir, file;

            inline bool operator==(const ReloadData &other) const {
                return type == other.type;
            }

            struct Hash {
                inline size_t operator()(const ReloadData &data) const {
                    return static_cast<size_t>(data.type);
                }
            };
        };

        filewatch::FileWatch<std::string> *fileWatcher;
        Timer timer;

        std::mutex mutexSet;
        std::unordered_set<ReloadData, ReloadData::Hash> pendingReloads;

        void handleResourceChange(const InitializationParameters &initializationParameters, const std::string &path);
        void reloadMaterial(const ReloadData &reloadData) const;
        void reloadImage(const ReloadData &reloadData) const;
        void reloadShader(const ReloadData &reloadData) const;
        void reloadResourcesFile(const ReloadData &reloadData) const;
    };
}