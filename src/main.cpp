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

        //Wybór funkcji testowej
        FunctionInfo funkcjaTestowaInfo = shubertInfo;

        //Wizualizacji funkcji testowej
        BenchFunction* benchHimmelBlau = new BenchFunction(vkEngine, funkcjaTestowaInfo, 10.);

        //Graficzna reprezentacja mrówek
        std::vector<AntRender*> antsToRender{};
        for(int i=0; i<3; i++)
            antsToRender.push_back(new AntRender(vkEngine, 0.5, {0.99, 0.0, 0.51}));

        //Graficzna reprezentacja mrowiska
        AntRender* nestRender = new AntRender(vkEngine, 1.5, {0.5, 0.99, 0.2});

        //Algorytm mrowiska
        const size_t dims = 2;
        // APIAntAlgorithm<dims>* antAlgorithm = new APIAntAlgorithm<dims>(static_cast<int>(antsToRender.size()), funkcjaTestowaInfo);
        APIAntAlgorithm<dims>* antAlgorithm = new APIAntAlgorithm<dims>(
            static_cast<int>(antsToRender.size())
            , std::function<double(double*, size_t)>(funkcjaTestowaInfo.functionPointer)
            , FunDomain<dims>(funkcjaTestowaInfo.xMin, funkcjaTestowaInfo.xMax)
        );

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
                std::vector<Point<dims>> listaPozycjiMrowek = antAlgorithm->getAntsPositions();
                Point<dims> p;

                for(int i=0; i<antsToRender.size(); i++){
                    p = listaPozycjiMrowek[i];
                    antsToRender[i]->getDrawInfo().transform.position = benchHimmelBlau->putOnFunction({p.pos[0], 0., p.pos[1]});
                }

                p = antAlgorithm->getNest();
                nestRender->getDrawInfo().transform.position = benchHimmelBlau->putOnFunction({p.pos[0], 0., p.pos[1]});
                
                timeToNextUpdate = 0.f;
            }

            //Rysowanie modeli
            DrawAllOfThem models{{benchHimmelBlau->getDrawInfo(), nestRender->getDrawInfo()}};
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
