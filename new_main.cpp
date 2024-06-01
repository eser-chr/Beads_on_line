#include <iostream>
#include <string>
#include <chrono>
#include "new.h"
using TIME_POINT=std::chrono::high_resolution_clock::time_point;

int main(int argc, char* argv[]){

    if(argc !=7){
        throw std::runtime_error("Invalid number of arguments");
    }

    int L = std::stoi(argv[1]);
    int N = std::stoi(argv[2]);
    int T = std::stoi(argv[3]);
    double kon = std::stoi(argv[4]);
    double koff = std::stoi(argv[5]);
    double kstep = std::stoi(argv[6]);

    FixedPopulationTasep simulation (L, N, T, kon, koff, kstep);

    
    TIME_POINT start = std::chrono::high_resolution_clock::now();
    simulation.simulation();
    TIME_POINT end = std::chrono::high_resolution_clock::now();

    std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
    
    
    
    std::cout<<"Execution time: "<< duration.count()<<std::endl;

}
