#include <iostream>
#include <vector>
#include "sir.hpp"

void print(std::ostream& os, Epidemic const& epidemic )  //cout è un tipo di os(?)
{
    //constexpr auto clear = "\033[2J";
    const auto N=world.side();
    
    //os << clear;
    os<<'+'<<std::string(N,'-')<<"+\n";
    for (int r=0; r!=N; ++r){
        os<<'|';
        for (int c=0; c!=N;++c){
            os<<(world.cell(r,c)==life::Cell::Alive ? '*':' ');
        }
        os<<'|'<<'\n';
    }
    os<<'+'<<std::string(N,'-')<<"+\n";
}




int main(){

    constexpr int World_size=20;
    life::World world(World_size);
    world.cell(1, 2) = life::Cell::Alive;
    world.cell(2, 2) = life::Cell::Alive;
    world.cell(3, 2) = life::Cell::Alive;


    for (int i=0; i!=4;++i) {
        world=evolve(world);
        print(std::cout,world);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

}