#pragma once
#include <glm/glm.hpp>
#include <functional>

typedef std::function<double(double, double)> FunctionPointer;

struct FunctionInfo {
    std::function<double(double, double)> functionPointer;

    float xMin;
    float xMax;
    float yMin;
    float yMax;
};


double himmelBlau(double a, double b);


const FunctionInfo himmelBlauInfo = { himmelBlau, -6.f, 6.f, -6.f, 6.f};