#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <vector>  
#include <random>
#include <chrono>
#include <algorithm> 
using namespace std;

//This is a git comment

// MAIN CONSTANTS OF THE SIMULATION
const int SIMS=10; // Number of simulations for a setup so that we have better statistics.

const int pi=1000; // We use divider to adjust prob to bind
const int epsilon=1000; // Similarly for the unbind
const int divider=1000000;

const int X=10000; // Number of positions i our road
const int T=20000; // Number of steps
const int N=1500; // Number of particles

const int period=T/10; // Number of blocks. At each block we will write the result
int kinesins[N][2]; // list for the position and state of each particle
bool grid[X]; // list if a position is captured or not
const int step_rate=9; // Num to define step probability Here 1/10

uniform_int_distribution<int> stepping(0,step_rate);
uniform_int_distribution<int> placer(0,X);
uniform_int_distribution<int> roller(0,divider);
#define PI 3.1415926


// Cleaning any previous data from other simulations.
void set_to_zero(){
    for(int i=0;i<N;i++){
		kinesins[i][0]=0;						
	}
	for(int i=0;i<X;++i){
			grid[i]=0;
	}
}

// Help function for initialization. Adds particles on the grid.
void add_kinesin(int kin, int x){
	kinesins[kin][0]=x;
	kinesins[kin][1]=1;
		grid[x]=1;
}

void initialize(){
	for(int i=0; i<N; i++){
		add_kinesin(i, i+3000);
	}
	
}

void writing(int& sim, int& k){
if(k%period==0){
			int i=k/period;
			std::string name_r = "R_"+std::to_string(i)+"_"+std::to_string(sim)+".txt";			
			ofstream file_r;			
			file_r.open(name_r,std::ofstream::trunc);
			for(int Kin=0; Kin<N;Kin++){					
				file_r<<kinesins[Kin][0]<<endl;
			}
			file_r.close();	
			}
		

}

void bind(int& a){
	int x=kinesins[a][0];
	if(grid[x]==false){
		kinesins[a][1]=1;
		grid[x]=true;
	}
}
void unbind(int& a){
	int x= kinesins[a][0];
	kinesins[a][1]=0;
	grid[x]=false;
}


// STep forward on the grid
void step(int& a,mt19937& engine){
	int x=kinesins[a][0];
	int type= kinesins[a][1];
	if(type==1){
		if(grid[x+1]==false && x+1<X){
				int rand = stepping(engine);
				if (rand==1){
					grid[x]=false;
					kinesins[a][0]=x+1;		
					grid[x+1]=true;
				}
			}
	}
}


// General move of the kinesin
void move(int& a, mt19937& engine){
	int rand =roller(engine);
	if(kinesins[a][1]==0){
		if(rand<pi){bind(a);}
	}else{
		if(rand<epsilon){unbind(a);}
	}
	step(a,engine);
}

// A process for a whole simulation
void simulation(int& sim, vector<int>& vec, mt19937& engine){
	for(int k=0;k<T;k++){
		writing(sim,k);
		std::shuffle(vec.begin(), vec.end(),engine);
		for(int kin:vec){
			int a=kin-1;
			move(a,engine);			
		}	
	}
}








int main(){
	unsigned seed =chrono::steady_clock::now().time_since_epoch().count();
	mt19937 engine(seed);
	vector<int> vec(N);
	
	iota(begin(vec),end(vec),1);
	for (int i=0; i<SIMS;i++){
		cout<<i<<endl;
	initialize();
	simulation(i,vec,engine);
		set_to_zero();
	}
}































