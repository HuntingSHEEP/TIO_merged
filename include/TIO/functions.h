#pragma once
#include <glm/glm.hpp>
#include <functional>

typedef std::function<double(double*, size_t)> FunctionPointer;

struct FunctionInfo {
    std::function<double(double*, size_t)> functionPointer;

    float xMin;
    float xMax;
    float yMin;
    float yMax;

    std::vector<std::pair<glm::vec2, double>> LUT2D;
    std::vector<std::pair<std::vector<double>, double>> LUT10D;
};


double f1(double* x, size_t dims);
double rastrigin(double* x, size_t dims);
double schwefel(double* x, size_t dims);
double booth(double* x, size_t dims);
double crossInTray(double* x, size_t dims);
double himmelBlau(double* x, size_t dims);
double sixHumpCamel(double* x, size_t dims);
double shubert(double* x, size_t dims);
double levy(double* x, size_t dims);
double f5(double* x, size_t dims);

const FunctionInfo f1Info = { f1, -10.f, 10.f, -10.f, 10.f, { { { 0.f, 0.f }, 0 } }, { { { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f }, 0 } } };
const FunctionInfo rastriginInfo = { rastrigin, -5.f, 5.f, -5.f, 5.f, { { { 0.f, 0.f }, 0 } }, { { { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f }, 0 } } };
const FunctionInfo schwefelInfo= { schwefel, -500.f, 500.f, -500.f, 500.f, { { { 420.968756f, 420.968756f }, 0 } }, { { { 420.968756f, 420.968756f, 420.968756f, 420.968756f, 420.968756f, 420.968756f, 420.968756f, 420.968756f, 420.968756f, 420.968756f }, 0 } } };
const FunctionInfo boothInfo = { booth, -10.f, 10.f, -10.f, 10.f, { { { 1.f, 3.f }, 0 } }, {} };
const FunctionInfo crossInTrayInfo = { 
    crossInTray
    , -2.f
    , 2.f
    , -2.f
    , 2.f
    ,   {
              { { 1.349406608602084f, 1.349406608602084f }, -2.062611870822739 }
            , { { -1.349406608602084f, 1.349406608602084f }, -2.062611870822739 }
            , { { -1.349406608602084f, -1.349406608602084f }, -2.062611870822739 }
            , { { 1.349406608602084f, -1.349406608602084f }, -2.062611870822739 }
        }
    , {}
    };
const FunctionInfo himmelBlauInfo = {
    himmelBlau
    , -5.f
    , 5.f
    , -5.f
    , 5.f
    ,   {
              { { -3.779310f, -3.283186f }, 0 }
            , { { -2.805118f, 3.131312f }, 0 }
            , { { 3.584428f, -1.848126f }, 0 }
            , { { 3.f, 2.f }, 0 }
        }
    , {}
    };
const FunctionInfo sixHumpCamelInfo = { sixHumpCamel, -2.f, 2.f, -2.f, 2.f, { { { 0.08984201368301331f, -0.7126564032704135f }, -1.031628453489877 }, { { -0.08984201368301331f, 0.7126564032704135f }, -1.031628453489877 } }, {} };
const FunctionInfo shubertInfo = { shubert, -2.f, 2.f, -2.f, 2.f, { { { -0.79167f, 1.41667f }, -186.7309 }, { { 0.79167, -1.41667 }, -186.7309 } }, {} };
const FunctionInfo levyInfo = { levy, -10.f, 10.f, -10.f, 10.f, { { { 1.f, 1.f }, 0 } }, { { { 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f }, 0 } } };
const FunctionInfo f5Info = { f5, -50.f, 50.f, -50.f, 50.f,  { { { -31.97833f, -31.97833f }, 0.998 } }, {} };

