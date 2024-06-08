#include <iostream>

#include "GraphicsEngine/VulkanEngine.h"
#include "GraphicsEngine/HelperStructures.h"
#include "TIO/BenchFunction.h"
#include "TIO/AntRender.h"
#include "TIO/functions.h"
#include "TIO/APIAntAlgorithm.hpp"
#include "TIO/AnthillRenderer.h"


int main(){
    try{
        //Wybór funkcji testowej
        FunctionInfo funkcjaTestowaInfo = himmelBlauInfo;

        //Liczba wymiarów
        const size_t dims = 10;

        //Liczba mrówek
        const int antsCount = 5;

        if(dims == 2)
        {
            //Algorytm mrówkowy
            APIAntAlgorithm<2>* antAlgorithm2 = new APIAntAlgorithm<2>(
                antsCount
                , std::function<double(double*, size_t)>(funkcjaTestowaInfo.functionPointer)
                , FunDomain<2>(funkcjaTestowaInfo.xMin, funkcjaTestowaInfo.xMax)
            );
            
            AnthillRenderer* anthillRenderer = new AnthillRenderer(funkcjaTestowaInfo, antsCount, antAlgorithm2);
            double timeToNextUpdate = 0.f;

            while(anthillRenderer->windowStillOpened()){
                anthillRenderer->enginePoll();

                if(timeToNextUpdate < 0.000025f){
                    timeToNextUpdate += anthillRenderer->getDeltaTime();
                }
                else if(!antAlgorithm2->finished()){
                    antAlgorithm2->update(funkcjaTestowaInfo);

                    anthillRenderer->update(antAlgorithm2->getAntsPositions(), antAlgorithm2->getNest());
                    timeToNextUpdate = 0.f;
                }

                anthillRenderer->draw();
            }
            anthillRenderer->finish();
        }
        else
        {
            //Algorytm mrówkowy
            APIAntAlgorithm<dims>* antAlgorithm = new APIAntAlgorithm<dims>(
                antsCount
                , std::function<double(double*, size_t)>(funkcjaTestowaInfo.functionPointer)
                , FunDomain<dims>(funkcjaTestowaInfo.xMin, funkcjaTestowaInfo.xMax)
            );

            while(!antAlgorithm->finished())
                antAlgorithm->update(funkcjaTestowaInfo);

            getchar();
        }
    }
    catch(const std::exception& e){std::cout<< "ERROR: "<<e.what()<<std::endl;}
    catch(...){std::cout<< "Unknown error"<<std::endl;}


    return 0;
}
