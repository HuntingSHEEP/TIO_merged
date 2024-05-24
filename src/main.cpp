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

        //Mrowisko
        Point nest{0., 0.};

        //Ant* ant = new Ant(&nest,  &std::function<double(double, double)>(himmelBlau) , 0);
        APIAntAlgorithm* antAlgorithm = new APIAntAlgorithm(1, std::function<double(double, double)>(himmelBlau), {-6, 6, -6, 6});


        //Przygotowanie sceny
        BenchFunction* benchHimmelBlau = new BenchFunction(vkEngine, himmelBlauTransformation);
        AntRender* lonelyAnt = new AntRender(vkEngine);


        //std::vector<AntRender> antsToRender{lo}
        

        float x = 5.9f;
        float z = 5.9f;

        //Główna pętla rysująca
        while(!glfwWindowShouldClose(vkEngine->window)){
            glfwPollEvents();
            vkEngine->updateTime();
            vkEngine->updateCamera(2.);

            /*
            x += static_cast<float>(6.f * vkEngine->deltaTime);
            if(6 < x){
                z += 0.2f;
                x = -6.f;
            }
            if(6 < z)
                z = -6.f;
            */
            if(!antAlgorithm->finished()){
                antAlgorithm->update();
                std::vector<Point> listaPozycjiMrowek = antAlgorithm->getAntsPositions();

                Point p = listaPozycjiMrowek[0];
                printf("Got from algortihm (%5.5f, %5.5f) ", p.x, p.y);
                lonelyAnt->getDrawInfo().transform.position = benchHimmelBlau->putOnFunction({p.x, 0., p.y});
                std::cout<<"  ant position: "<<glm::to_string(lonelyAnt->getDrawInfo().transform.position)<<std::endl;
            }
            


            


            //Rysowanie modeli
            DrawAllOfThem models{{benchHimmelBlau->getDrawInfo(), lonelyAnt->getDrawInfo()}};
            vkEngine->drawFrameAnother(models);
        }
        vkDeviceWaitIdle(vkEngine->device);
    }
    catch(const std::exception& e){std::cout<< "ERROR: "<<e.what()<<std::endl;}
    catch(...){std::cout<< "Unknown error"<<std::endl;}


    return 0;
}
