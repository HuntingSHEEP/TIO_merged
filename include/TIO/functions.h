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


double f1(double a, double b);
double rastrigin(double a, double b);
double schwefel(double a, double b);
double booth(double a, double b);
double crosInTrey(double a, double b);
double himmelBlau(double a, double b);
double sixHumpCamel(double a, double b);
double shubert(double a, double b);
double levy(double a, double b);
double f5LisieNory(double a, double b);



const FunctionInfo himmelBlauInfo = { himmelBlau, -6.f, 6.f, -6.f, 6.f};
const FunctionInfo sixHumpCamelInfo = { sixHumpCamel, -2.f, 2.f, -1.f, 1.f};