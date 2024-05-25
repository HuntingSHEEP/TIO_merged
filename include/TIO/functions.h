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
};


double f1(double* x, size_t dims);
double rastrigin(double* x, size_t dims);
double schwefel(double* x, size_t dims);
double booth(double* x, size_t dims);
double crosInTrey(double* x, size_t dims);
double himmelBlau(double* x, size_t dims);
double sixHumpCamel(double* x, size_t dims);
double shubert(double* x, size_t dims);
double levy(double* x, size_t dims);
double f5LisieNory(double* x, size_t dims);




const FunctionInfo rastriginInfo = { rastrigin, -5.f, 5.f, -5.f, 5.f };
const FunctionInfo sixHumpCamelInfo = { sixHumpCamel, -2.f, 2.f, -2.f, 2.f };
const FunctionInfo boothInfo = { booth, -10.f, 10.f, -10.f, 10.f };
const FunctionInfo schwefelInfo= { schwefel, -500.f, 500.f, -500.f, 500.f };
const FunctionInfo shubertInfo = { shubert, -2.f, 2.f, -2.f, 2.f };