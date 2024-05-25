#include "TIO/functions.h"


double himmelBlau(double a, double b){
    return (pow(a * a + b - 11, 2) + pow(a + b * b - 7, 2));
};

double sixHumpCamel(double a, double b) {
    double term1 = (4.0 - 2.1 * a * a + (a * a * a * a) / 3.0) * a * a;
    double term2 = a * b;
    double term3 = (-4.0 + 4.0 * b * b) * b * b;

    return term1 + term2 + term3;
}
