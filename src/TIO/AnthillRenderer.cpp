#include "TIO/AnthillRenderer.h"



AnthillRenderer::AnthillRenderer(FunctionInfo funkcjaTestowaInfo, const int antsC, APIAntAlgorithm<2>* antAlg) :
antAlgorithm(antAlg), antsCount(antsC){

    //Inicjowanie silnika renderującego
    vkEngine = new VulkanEngine();
    vkEngine->setupCallbacks(vkEngine->window);

    //Wizualizacja funkcji testowej
    benchHimmelBlau = new BenchFunction(vkEngine, funkcjaTestowaInfo, 10.);

    //Graficzna reprezentacja mrówek
    antsToRender = {};
    for(int i=0; i<antsCount; i++)
        antsToRender.push_back(new AntRender(vkEngine, 0.5, {0.99, 0.0, 0.51}));

    //Graficzna reprezentacja mrowiska
    nestRender = new AntRender(vkEngine, 1.5, {0.5, 0.99, 0.2});
};

bool AnthillRenderer::windowStillOpened(){
    return !glfwWindowShouldClose(vkEngine->window);
};

double AnthillRenderer::getDeltaTime(){
    return vkEngine->deltaTime;
};

void AnthillRenderer::enginePoll(){
    glfwPollEvents();
    vkEngine->updateTime();
    vkEngine->updateCamera(2.);
};

void AnthillRenderer::update(std::vector<Point<2>> listaPozycjiMrowek, Point<2> nestPosition){
    Point<2> p;

    for(int i=0; i<antsToRender.size(); i++){
        p = listaPozycjiMrowek[i];
        antsToRender[i]->getDrawInfo().transform.position = benchHimmelBlau->putOnFunction({p.pos[0], 0., p.pos[1]});
    }

    p = nestPosition;
    nestRender->getDrawInfo().transform.position = benchHimmelBlau->putOnFunction({p.pos[0], 0., p.pos[1]});
        
};

void AnthillRenderer::draw(){
    //Rysowanie modeli
    DrawAllOfThem models{{benchHimmelBlau->getDrawInfo(), nestRender->getDrawInfo()}};
    for(auto& a : antsToRender)
        models.allModels.push_back(a->getDrawInfo());

    vkEngine->drawFrameAnother(models);
};

void AnthillRenderer::finish(){
    vkDeviceWaitIdle(vkEngine->device);
};
