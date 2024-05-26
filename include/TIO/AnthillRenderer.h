#pragma once 
#include "GraphicsEngine/VulkanEngine.h"
#include "GraphicsEngine/HelperStructures.h"
#include "TIO/BenchFunction.h"
#include "TIO/AntRender.h"
#include "TIO/functions.h"
#include "TIO/APIAntAlgorithm.hpp"

class AnthillRenderer{
public:
    AnthillRenderer(FunctionInfo funkcjaTestowaInfo, const int antsC, APIAntAlgorithm<2>* antAlg);
    bool windowStillOpened();
    double getDeltaTime();
    void enginePoll();
    void update(std::vector<Point<2>> listaPozycjiMrowek, Point<2> nestPosition);
    void draw();
    void finish();
    
private:
    VulkanEngine* vkEngine;
    std::vector<AntRender*> antsToRender;
    AntRender* nestRender;

    BenchFunction* benchHimmelBlau;
    APIAntAlgorithm<2>* antAlgorithm;

    int antsCount;
};

