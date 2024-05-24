#include <iostream>

#include "GraphicsEngine/VulkanEngine.h"
#include "GraphicsEngine/HelperStructures.h"
#include "TIO/BenchFunction.h"
#include "TIO/AntRender.h"
#include "TIO/functions.h"

#include "TIO/APIAntAlgorithm.hpp"

int main(){
    try{
        //Inicjowanie silnika renderującego
        VulkanEngine* vkEngine = new VulkanEngine();
        vkEngine->setupCallbacks(vkEngine->window);

        //Przygotowanie sceny
        BenchFunction* benchHimmelBlau = new BenchFunction(vkEngine, himmelBlauTransformation);
        std::vector<AntRender*> antsToRender{};

        for(int i=0; i<3; i++)
            antsToRender.push_back(new AntRender(vkEngine));

        //Mrowisko
        Point nest{0., 0.};
        APIAntAlgorithm* antAlgorithm = new APIAntAlgorithm(static_cast<int>(antsToRender.size()), std::function<double(double, double)>(himmelBlau), {-6, 6, -6, 6});


        double timeToNextUpdate = 0.f;

        //Główna pętla rysująca
        while(!glfwWindowShouldClose(vkEngine->window)){
            glfwPollEvents();
            vkEngine->updateTime();
            vkEngine->updateCamera(2.);


            if(timeToNextUpdate < 0.25f)
            {
                timeToNextUpdate += vkEngine->deltaTime;
            }
            else if(!antAlgorithm->finished())
            {
                antAlgorithm->update();
                std::vector<Point> listaPozycjiMrowek = antAlgorithm->getAntsPositions();

                for(int i=0; i<antsToRender.size(); i++){
                    Point p = listaPozycjiMrowek[i];
                    antsToRender[i]->getDrawInfo().transform.position = benchHimmelBlau->putOnFunction({p.x, 0., p.y});
                }
                
                timeToNextUpdate = 0.f;
            }

            //Rysowanie modeli
            DrawAllOfThem models{{benchHimmelBlau->getDrawInfo()}};
            for(auto& a : antsToRender)
                models.allModels.push_back(a->getDrawInfo());

            vkEngine->drawFrameAnother(models);
        }
        vkDeviceWaitIdle(vkEngine->device);
    }
    catch(const std::exception& e){std::cout<< "ERROR: "<<e.what()<<std::endl;}
    catch(...){std::cout<< "Unknown error"<<std::endl;}


    return 0;
}
