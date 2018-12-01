#include "integrate.h"

#include <omp.h>
#include <cmath>

double
integrate_sequential(std::function<double(double)> integrand, double a, double step_size, int step_count) {

    double acc = 0.0;

    for(int i = 0; i <= step_count; i ++){
        acc += integrand(a + (i * step_size) + (step_size/2));
    }
    return acc;
}

double
integrate_omp_critical(std::function<double(double)> integrand, double a, double step_size, int step_count) {
    double acc = 0.0;

#pragma omp parallel
    {
        double tmp = 0.0;
#pragma omp for
        for (int i = 0; i <= step_count; i++) {
            tmp += integrand(a + (i * step_size) + (step_size / 2));
        }
#pragma omp critical
        acc += tmp;
    }

    return acc;
}

double
integrate_omp_atomic(std::function<double(double)> integrand, double a, double step_size, int step_count) {
    double acc = 0.0;

#pragma omp parallel
    {
        double tmp = 0.0;
#pragma omp for
        for (int i = 0; i <= step_count; i++) {
            tmp += integrand(a + (i * step_size) + (step_size / 2));
        }
#pragma omp atomic
        acc += tmp;
    }

    return acc;
}

double integrate_omp_reduction(std::function<double(double)> integrand, double a, double step_size, int step_count) {
    double acc = 0.0;

#pragma omp parallel for reduction(+:acc)
    for(int i = 0; i <= step_count; i ++){
        acc += integrand(a + (i * step_size) + (step_size/2));
    }
    return acc;
}

double integrate_omp_for_static(std::function<double(double)> integrand, double a, double step_size, int step_count) {
    double acc = 0.0;

#pragma omp parallel for reduction(+:acc) schedule(static)
    for(int i = 0; i <= step_count; i ++){
        acc += integrand(a + (i * step_size) + (step_size/2));
    }

    return acc;
}

double integrate_omp_for_dynamic(std::function<double(double)> integrand, double a, double step_size, int step_count) {
    double acc = 0.0;

#pragma omp parallel
    {
        double tmp = 0.0;
#pragma omp for schedule(dynamic)
        for (int i = 0; i <= step_count; i++) {
            tmp += integrand(a + (i * step_size) + (step_size / 2));
        }
#pragma omp atomic
        acc += tmp;
    }

    return acc;
}
