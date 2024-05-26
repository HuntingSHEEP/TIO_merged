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
        const size_t dims = 2;

        //Liczba mrówek
        const int antsCount = 5;

        //Algorytm mrówkowy
        APIAntAlgorithm<dims>* antAlgorithm = new APIAntAlgorithm<dims>(
            antsCount
            , std::function<double(double*, size_t)>(funkcjaTestowaInfo.functionPointer)
            , FunDomain<dims>(funkcjaTestowaInfo.xMin, funkcjaTestowaInfo.xMax)
        );

        AnthillRenderer* anthillRenderer = new AnthillRenderer(funkcjaTestowaInfo, antsCount, antAlgorithm);
        double timeToNextUpdate = 0.f;

        while(anthillRenderer->windowStillOpened()){
            anthillRenderer->enginePoll();

            if(timeToNextUpdate < 0.25f){
                timeToNextUpdate += anthillRenderer->getDeltaTime();
            }
            else if(!antAlgorithm->finished()){
                antAlgorithm->update();

                // After the algorithm finished its work, display the best place found and it's coordinates
                if(antAlgorithm->finished())
                {
                    auto [point, value] = antAlgorithm->getBest();
                    std::cout << "Best value found in point (";
                    for(int i = 0; i < dims; i++)
                    {
                        std::cout << point.pos[i] << ( i != dims - 1 ? ", " : "");
                    }
                    std::cout << ") with value: " << value << ".\n";
                }

                anthillRenderer->update(antAlgorithm->getAntsPositions(), antAlgorithm->getNest());
                timeToNextUpdate = 0.f;
            }

            anthillRenderer->draw();
        }

        anthillRenderer->finish();
    }
    catch(const std::exception& e){std::cout<< "ERROR: "<<e.what()<<std::endl;}
    catch(...){std::cout<< "Unknown error"<<std::endl;}


    return 0;
}
