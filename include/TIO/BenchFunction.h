#pragma once 
#include "GraphicsEngine/VulkanEngine.h"
#include "GraphicsEngine/HelperStructures.h"
#include "GraphicsEngine/VkDraw.h"

#include <functional>
typedef std::function<double(double, double)> FunctionPointer;


class BenchFunction : public VkDraw{
public:
    BenchFunction(VulkanEngine* app, FunctionPointer func);

    glm::vec3 putOnFunction(glm::vec3 position);
    void wrapperTransform(glm::vec3& vertex);
    void normalize(std::vector<Vertex>& vertices, float scale);
    void colorizeByDepth(std::vector<Vertex>& vertices);
    void mapBenchFunction(VulkanEngine* app, Model* model);

    DrawModelInfo& getDrawInfo() override;

    
private:
    FunctionPointer transformFunction;
    Transform transform;
    DrawModelInfo drawInfo;
    float modifier;
};