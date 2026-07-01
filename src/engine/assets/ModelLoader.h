#pragma once

#include <string>
#include <memory>
#include "engine/render/Mesh.h"

namespace engine
{
    class ModelLoader
    {
    public:
        static std::unique_ptr<Mesh> Load(const std::string &path);
    };
} // namespace engine
