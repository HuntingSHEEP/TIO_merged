/*
    https://www.sfu.ca/~ssurjano/optimization.html
*/

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

double levy(double* x, size_t dims) {
    double wynik = 0.0;

    // Wstępne obliczenie wartości w_i
    double* w = new double[dims];
    for (size_t i = 0; i < dims; ++i) {
        w[i] = 1 + (x[i] - 1) / 4.0;
    }

    // Składowa początkowa
    wynik += sin(PI * w[0]) * sin(PI * w[0]);

    // Składowe środkowe
    for (size_t i = 0; i < dims - 1; ++i) {
        wynik += (w[i] - 1) * (w[i] - 1) * (1 + 10 * sin(PI * w[i] + 1) * sin(PI * w[i] + 1));
    }

    // Składowa końcowa
    wynik += (w[dims - 1] - 1) * (w[dims - 1] - 1) * (1 + sin(2 * PI * w[dims - 1]) * sin(2 * PI * w[dims - 1]));

    // Zwolnienie pamięci
    delete[] w;

    return wynik;
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

double crossInTray(double* x, size_t dims) {
    double x1 = x[0];
    double x2 = x[1];

    double term1 = sin(x1) * sin(x2);
    double term2 = exp(abs(100 - sqrt(x1 * x1 + x2 * x2) / PI));
    double result = -0.0001 * pow(abs(term1 * term2) + 1, 0.1);
    return result;
}

double himmelBlau(double* x, size_t dims) {
    double x1 = x[0];
    double x2 = x[1];

    double term1 = (x1 * x1 + x2 - 11);
    double term2 = (x1 + x2 * x2 - 7);
    double result = term1 * term1 + term2 * term2;

    return result;
}