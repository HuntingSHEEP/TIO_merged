#include "TIO/functions.h"
const double PI = 3.141592653589793238462643383279502884197;


//FUNKCJE WIELOWYMIAROWE__###############################################
//_______________________________________________________________________

double rastrigin(double* x, size_t dims){
    double wynik = 10. * dims;

    for(size_t i=0; i<dims; i++)
        wynik += x[i]*x[i] - 10*cos(2*PI*x[i]);
    
    return wynik;
}

double schwefel(double* x, size_t dims){
    double wynik = 418.9829 * dims;

    double suma = 0;
    for(size_t i=0; i<dims; i++)
        suma += x[i] * sin( sqrt( abs( x[i] )));

    return wynik - suma;
}



//FUNKCJE TYLKO DWUWYMIAROWE__###########################################
//_______________________________________________________________________

double booth(double* x, size_t dims){
    return pow((x[0] + 2*x[1] - 7), 2) + pow((2*x[0] + x[1] -5), 2);
}

double sixHumpCamel(double* x, size_t dims) {
    double a = x[0];
    double b = x[1];

    double term1 = (4.0 - 2.1 * a * a + (a * a * a * a) / 3.0) * a * a;
    double term2 = a * b;
    double term3 = (-4.0 + 4.0 * b * b) * b * b;

    return term1 + term2 + term3;
};

double shubert(double* x, size_t dims){
    double sum1 = 0;
    double sum2 = 0;

    for(size_t i=0; i<5; i++){
        sum1 += i * cos( (i+1) * x[0] + i);
        sum2 += i * cos( (i+1) * x[1] + i);
    }

    return sum1 * sum2;     
}