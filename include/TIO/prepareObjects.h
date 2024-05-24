#pragma once 
#include "GraphicsEngine/VulkanEngine.h"
#include "GraphicsEngine/HelperStructures.h"
#include "TIO/BenchFunction.h"
#include "TIO/functions.h"
#include <vector>

inline DrawAllOfThem prepareObjects(VulkanEngine* app) {
    BenchFunction* himmelBlau = new BenchFunction(app, himmelBlauTransformation);

    std::vector<DrawModelInfo> allModels{};
    allModels.push_back(himmelBlau->getDrawInfo());
    
    return { allModels };
}
