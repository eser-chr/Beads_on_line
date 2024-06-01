#ifndef NEW_H
#define NEW_H


#include <random>
#include <vector>
#include <numeric> // For partial sum
#include <algorithm>
#include <stdexcept>
#include <iostream>

#ifdef DEBUG
#define PRINT(msg, msg2) std::cout<< (msg)<<" "<<(msg2)<<std::endl;
#else
#define PRINT(msg, msg2)
#endif 


class FixedPopulationTasep{
public:

FixedPopulationTasep(int L, int N, int T, double kon, double koff, double kstep);

void bind(int side);
void unbind(int side);
void step(int side);

void fix_boundaries();
void fix_cumsum();

void iteration();
void append_to_data();

void simulation();




private:
    int L; // with 2 ghost layers
    int N; //population
    int T;
    double kon;
    double koff;
    double kstep;
    std::vector<double> propensities;
    std::vector<double> sum_propensities;
    std::vector<bool> grid;
    std::vector<std::vector<bool>> DATA;
    std::vector<double> TIMES;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<double> dis;
    double next_write_time = writing_period;

    double time = 0;
    int BOUND = 0;
    int UNBOUND = N;
    const int l_ghost = 1;
    const int r_ghost = 2;
    const int ghost = l_ghost + r_ghost;
    const int N_actions = 3;
    const double writing_period = 0.1;
    
    // Displacement
    const int __BIND = 0 + l_ghost;
    const int __UNBIND = (L+ghost);
    const int __STEP = 2*(L+ghost);

    int _action, _side, _index;
    double r1, r2, dt;
};

#endif