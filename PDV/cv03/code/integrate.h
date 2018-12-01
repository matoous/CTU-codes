//
// Created by karel on 6.3.18.
//

#include <functional>

#ifndef CODE_INTEGRATE_H
#define CODE_INTEGRATE_H

double integrate_sequential(std::function<double(double)> integrand, double a, double step_size, int step_count);
double integrate_omp_critical(std::function<double(double)> integrand, double a, double step_size, int step_count);
double integrate_omp_atomic(std::function<double(double)> integrand, double a, double step_size, int step_count);
double integrate_omp_reduction(std::function<double(double)> integrand, double a, double step_size, int step_count);

double integrate_omp_for_static(std::function<double(double)> integrand, double a, double step_size, int step_count);
double integrate_omp_for_dynamic(std::function<double(double)> integrand, double a, double step_size, int step_count);

#endif //CODE_INTEGRATE_H
