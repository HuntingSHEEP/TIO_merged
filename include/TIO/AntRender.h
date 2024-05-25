#pragma once 
#include "GraphicsEngine/VulkanEngine.h"
#include "GraphicsEngine/HelperStructures.h"


class AntRender{
public:
    AntRender(VulkanEngine* vkEngine, float antSize, glm::vec3 color);
    DrawModelInfo createModel(VulkanEngine* vkEngine, Model model, Pipeline pipeline, Texture texture, Transform transform);
    DrawModelInfo& getDrawInfo();
private:
    DrawModelInfo modelInfo;
};