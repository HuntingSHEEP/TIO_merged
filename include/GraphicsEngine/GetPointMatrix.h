#pragma once 
#include "GraphicsEngine/VulkanEngine.h"
#include "GraphicsEngine/HelperStructures.h"
#include <vector>
#include <iostream>


inline CreateModelInfo getPointMatrix(int size, float step) {
    std::vector<Vertex> punkty;
    std::vector<uint32_t> indeksy;

    for(int x=0; x<size; x++)
        for (int z = 0; z < size; z++) {
            Vertex v = { {x*step, 0.0, z*step}, {1, 0.1, 1} };
            punkty.push_back(v);
            indeksy.push_back(x*size + z);
        }

    return {punkty, indeksy};
}


inline Model getPointMatrix(VulkanEngine* vkEngine, CreateModelInfo modelInfo) {
    return vkEngine->initModel(modelInfo.punkty, modelInfo.indeksy);
}

inline Model getPointMatrix(VulkanEngine* vkEngine, int size, float step) {
    CreateModelInfo modelInfo = getPointMatrix(size, step);
    return vkEngine->initModel(modelInfo.punkty, modelInfo.indeksy);
}


inline Model getPointMatrix(VulkanEngine* vkEngine, float xMin, float xMax, float yMin, float yMax, int nX, int nY) {
    std::vector<Vertex> punkty;
    std::vector<uint32_t> indeksy;
    
    float wymiarX = abs(xMax - xMin);
    float skokX = wymiarX/nX;

    float wymiarY = abs(yMax - yMin);
    float skokY = wymiarY/nY;

    glm::vec3 startPoint = {xMin, .0, yMin};

    for(int x=0; x< nX+1; x++)
        for (int y=0; y< nY+1; y++) {
            glm::vec3 point = glm::vec3(x*skokX, 0.0, y*skokY) + startPoint;
            glm::vec3 color = {0, .99, .2};
            Vertex v = { point, color };
            int index = x*nX + y;

            //std::cout<<index << "  " << glm::to_string(v.pos) <<std::endl;
            punkty.push_back(v);
            indeksy.push_back(index);
        }
    
    return vkEngine->initModel(punkty, indeksy);
}




inline Model getPointMatrix(VulkanEngine* vkEngine, float xMin, float xMax, int nX) {
    return getPointMatrix(vkEngine, xMin, xMax, xMin, xMax, nX, nX);
}
