#include <chrono>
#include <iostream>
#include <thread>
#include "sirII.hpp"


void print(std::ostream& os, life::Population const& population )
{
    //constexpr auto clear = "\033[2J";
    const auto N=population.side();
    
    //os << clear;
    os<<'+'<<std::string(N,'-')<<"+\n";
    for (int r=0; r!=N; ++r){
        os<<'|';
        for (int c=0; c!=N;++c){
            os<<(population.human(r,c)==life::Human::Alive ? '*':' '); //da modificare, suscettibili O, infetti X, rimossi spazio vuoto
        }
        os<<'|'<<'\n';
    }
    os<<'+'<<std::string(N,'-')<<"+\n";
}




int main(){

    constexpr int Population.side()=20;
    life::Population population(Population.side());
    population.human(1, 2) = life::Human::Alive;
    population.human(2, 2) = life::Human::Alive;
    population.human(3, 2) = life::Human::Alive;

s
    for (int i=0; i!=4;++i) {
        population=evolve(population);
        print(std::cout,population);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

}