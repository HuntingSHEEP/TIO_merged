#include "TIO/functions.h"

void himmelBlauTransformation(glm::vec3& vertex) {
    glm::vec3 p = vertex;
    vertex.y = static_cast<float>(himmelBlau(p.x, p.z));
};

double himmelBlau(double a, double b){
    return (pow(a * a + b - 11, 2) + pow(a + b * b - 7, 2));
};