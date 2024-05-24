#pragma once 
#include "GraphicsEngine/VulkanEngine.h"
#include "GraphicsEngine/HelperStructures.h"
#include "GraphicsEngine/VkDraw.h"
#include "TIO/functions.h"



class BenchFunction : public VkDraw{
public:
    BenchFunction(VulkanEngine* app, FunctionInfo info);

    glm::vec3 putOnFunction(glm::vec3 position);
    void wrapperTransform(glm::vec3& vertex);
    void normalize(std::vector<Vertex>& vertices, float scale);
    void colorizeByDepth(std::vector<Vertex>& vertices);
    void mapBenchFunction(VulkanEngine* app, Model* model);

    DrawModelInfo& getDrawInfo() override;

    
private:
    FunctionPointer transformFunction;
    FunctionInfo functionInfo;

    Transform transform;
    DrawModelInfo drawInfo;
    float modifier;
};