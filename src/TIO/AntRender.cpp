#include "TIO/AntRender.h"

AntRender::AntRender(VulkanEngine* vkEngine){
    Texture paper = vkEngine->initTexture({ "../resources/textures/whitePaper2.jpg" });
    Model sphere = vkEngine->initModel({ "../resources/models/sphere.obj" });
    Pipeline pipeline = vkEngine->initPipeline({ "../resources/shaders/vert.spv" , "../resources/shaders/frag.spv", VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST});
   
   
    Transform transform{{ -6., 0., 0. }, { 0.2, 0.2, 0.2 }, { 0., 0., 1. }, .0f};
    modelInfo = createModel(vkEngine, sphere, pipeline, paper, transform);
}

DrawModelInfo& AntRender::getDrawInfo(){
    return modelInfo;
}

DrawModelInfo AntRender::createModel(VulkanEngine* vkEngine, Model model, Pipeline pipeline, Texture texture, Transform transform){
    UniformBuffer wallBufferS = vkEngine->initUniformBuffer();
    DescriptorSets wallDescriptorS = vkEngine->initDescriptorSet(texture, wallBufferS);

    DrawModelInfo drawWallS{};
    drawWallS.model = model;
    drawWallS.transform = transform;
    drawWallS.pipeline = pipeline;
    drawWallS.buffer = wallBufferS;
    drawWallS.descriptorSets = wallDescriptorS;

    return drawWallS;
};
