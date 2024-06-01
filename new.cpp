#include "new.h"


FixedPopulationTasep::FixedPopulationTasep(int L, int N, int T, double kon, double koff, double kstep)
:L(L), N(N), T(T), kon(kon), koff(koff), kstep(kstep),gen(42), dis(0.0, 1.0), BOUND(0), UNBOUND(N){
    propensities.resize(N_actions*(L+ghost), 0);
    sum_propensities.resize(N_actions*(L+ghost));
    std::fill(propensities.begin()+1, propensities.begin()+L-2, kon);
    std::partial_sum(propensities.begin(), propensities.end(), sum_propensities.begin());

    grid.resize(L+ghost, 0);
}


void FixedPopulationTasep::bind(int side){
    BOUND+=1;
    UNBOUND-=1;
    propensities[__BIND + side] = 0;
    propensities[__UNBIND + side] = koff;
    propensities[__STEP + side] = (double) kstep*grid[l_ghost + side];
    propensities[__STEP + side-1] = 0.0;

    grid[l_ghost+side] = 1;

    for(size_t i = 0+l_ghost; i<L-r_ghost; i++)
        propensities[i]= kon*(double) UNBOUND /(double) N;    //calc once

}

void FixedPopulationTasep::unbind(int side){
    BOUND-=1;
    UNBOUND+=1;
    propensities[__BIND + side] = kon ;
    propensities[__UNBIND + side] = 0.0;
    propensities[__STEP + side] = 0.0;
    propensities[__STEP + side-1] =(double) kstep*grid[l_ghost+ side-1];

    grid[l_ghost + side] = 0;
    for(size_t i = 0+l_ghost; i<L-r_ghost; i++)
        propensities[i]= kon*(double) UNBOUND /(double) N;    //calc once

}


void FixedPopulationTasep::step(int side){
    propensities[__BIND + side +1 ] = propensities[__BIND +side];
    propensities[__BIND + side] = 0.0;

    propensities[__UNBIND + side] =0;
    propensities[__UNBIND + side +1] = koff;

    propensities[__STEP + side -1] = (double) kstep*grid[l_ghost+side-1];
    propensities[__STEP + side] = 0.0;
    propensities[__STEP + side +1] = (double) kstep*grid[l_ghost+side+2];

    grid[l_ghost + side] = 0;
    grid[l_ghost + side +1] = 1;
}

void FixedPopulationTasep::fix_boundaries(){
    for(size_t left_ghost =0; left_ghost<l_ghost; left_ghost++){
        propensities[__BIND-l_ghost+left_ghost] = 0;
        propensities[__UNBIND-l_ghost+left_ghost] = 0;
        propensities[__STEP-l_ghost+left_ghost] = 0;
    }    

    for(size_t right_ghost =0; right_ghost<r_ghost; right_ghost++){
        propensities[__BIND+L+right_ghost] = 0;
        propensities[__UNBIND+L+right_ghost] = 0;
        propensities[__STEP+L+right_ghost] = 0;
    }
    
}

void FixedPopulationTasep::fix_cumsum(){
    std::partial_sum(propensities.begin(), propensities.end(), sum_propensities.begin());
}



void FixedPopulationTasep::iteration(){ //Possible std::async
    r1 = dis(gen);
    dt = (1.0/sum_propensities.back()) * log(1/r1);
    time += dt;

    r2 = dis(gen) * sum_propensities.back();

    _index = std::distance(sum_propensities.begin(), 
                    std::upper_bound(sum_propensities.begin(), sum_propensities.end(), r2));
    
    _action = _index / (L+ghost);
    _side = _index % (L+ghost);

    PRINT(_action, _side)
    PRINT(BOUND, UNBOUND)
    PRINT(dt, r2)
    // PRINT(r2, sum_propensities.back())

    switch(_action){
        case 0 :
            bind(_side);
            break;
        case 1:
            unbind(_side);
            break;
        case 2:
            step(_side);
            break;
        default:
            PRINT(r2, "")
            PRINT(_index, "")
            PRINT(_side, "")
            throw std::runtime_error("An error occurred in switch statement");
    }

    fix_boundaries();
    fix_cumsum();
}


void FixedPopulationTasep::append_to_data(){ // Need to resize at the constructor to avoid rellocation
    DATA.push_back(grid);
    TIMES.push_back(time);
}

void FixedPopulationTasep::simulation(){
    while(time<T){
        iteration();
        if (next_write_time<time){
            append_to_data();
            next_write_time += writing_period;
        }
    }

}






