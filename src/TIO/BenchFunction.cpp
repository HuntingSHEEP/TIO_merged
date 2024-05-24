#include "TIO/BenchFunction.h"
#include "GraphicsEngine/GetPointMatrix.h"


BenchFunction::BenchFunction(VulkanEngine* vkEngine,  FunctionInfo info) 
: transformFunction(info.functionPointer), functionInfo(info){
    Transform t{};
    t.position = { 0., 0., 0. };
    t.scale = { 1., 1., 1. };
    t.rotationAxis = { 0., 0., 1. };
    t.rotationAngle = 0.f;

    Pipeline pipeline = vkEngine->initPipeline({ "../resources/shaders/vert.spv" , "../resources/shaders/frag.spv", VK_PRIMITIVE_TOPOLOGY_POINT_LIST });
    Model model = getPointMatrix(vkEngine, functionInfo.xMin, functionInfo.xMax, functionInfo.yMin, functionInfo.yMax, 100, 100);
    Texture paper = vkEngine->initTexture({ "../resources/textures/whitePaper2.jpg" });

    UniformBuffer uBuffer = vkEngine->initUniformBuffer();
    DescriptorSets dSet = vkEngine->initDescriptorSet(paper, uBuffer);

    mapBenchFunction(vkEngine, &model);
    this->drawInfo = {model, t, pipeline, uBuffer, dSet};
};

void BenchFunction::wrapperTransform(glm::vec3& vertex){
    glm::vec3 p = vertex;
    vertex.y = static_cast<float>(transformFunction(p.x, p.z));
};


void BenchFunction::mapBenchFunction(VulkanEngine* vkEngine, Model* model) {
    std::vector<Vertex> vertices = model->vertices;

    for (auto& v : vertices)
        wrapperTransform(v.pos);
    
    normalize(vertices, 10.);
    colorizeByDepth(vertices);
    
    vkEngine->updateModel(model, vertices);
};


void BenchFunction::normalize(std::vector<Vertex>& vertices, float scale){
    float yMax = 0;
    float yMin = 0;

    for (auto v : vertices){
        float y = v.pos.y; 
        if (y < yMin)
            yMin = y;
        else if (yMax < y)
            yMax = y;
    }

    float range = yMax - yMin;
    this->modifier = scale/range;

    for (auto& v : vertices)
        v.pos.y *= modifier;
}

glm::vec3 BenchFunction::putOnFunction(glm::vec3 position){
    wrapperTransform(position);

    position.y *= modifier;
    return position;
}


void BenchFunction::colorizeByDepth(std::vector<Vertex>& vertices) {
    // Znajdujemy minimalną i maksymalną głębokość
    float yMin = vertices[0].pos.y;
    float yMax = vertices[0].pos.y;
    for (const auto& vertex : vertices) {
        if (vertex.pos.y < yMin)
            yMin = vertex.pos.y;
        if (vertex.pos.y > yMax)
            yMax = vertex.pos.y;
    }

    // Kolorujemy wierzchołki na podstawie ich głębokości w osi Y
    for (auto& vertex : vertices) {
        // Interpolacja kolorów w przestrzeni HSL
        float t = (vertex.pos.y - yMin) / (yMax - yMin);
        glm::vec3 hslColor(240.0f * (1.0f - t), 1.0f, 0.5f); // Barwa, nasycenie, jasność

        // Konwersja z HSL do RGB
        float hue = hslColor.x;
        float saturation = hslColor.y;
        float lightness = hslColor.z;

        float chroma = (1.0f - fabs(2.0f * lightness - 1.0f)) * saturation;
        float huePrime = hue / 60.0f;
        float x = chroma * (1.0f - fabs(fmod(huePrime, 2.0f) - 1.0f));

        glm::vec3 rgbColor;
        if (0 <= huePrime && huePrime < 1)
            rgbColor = glm::vec3(chroma, x, 0);
        else if (1 <= huePrime && huePrime < 2)
            rgbColor = glm::vec3(x, chroma, 0);
        else if (2 <= huePrime && huePrime < 3)
            rgbColor = glm::vec3(0, chroma, x);
        else if (3 <= huePrime && huePrime < 4)
            rgbColor = glm::vec3(0, x, chroma);
        else if (4 <= huePrime && huePrime < 5)
            rgbColor = glm::vec3(x, 0, chroma);
        else if (5 <= huePrime && huePrime < 6)
            rgbColor = glm::vec3(chroma, 0, x);

        float m = lightness - 0.5f * chroma;
        vertex.color = rgbColor + glm::vec3(m);
    }
}

    

DrawModelInfo& BenchFunction::getDrawInfo(){
    return drawInfo;
};



