#include <iomanip>
#include <iostream>
#include "sir.hpp"
#include <stdexcept>


void print(std::vector<State> const& states)
{

  std::cout << "+-----------+-----------+-----------+\n"
               "|     S     |     I     |     R     |\n"
               "+-----------+-----------+-----------+\n"
            << std::fixed;
  for (auto& state : states) {
    std::cout << "| " << std::setw(9) << state.S << " | " << std::setw(9) << state.I << " | "<< std::setw(9) << state.R<<" |\n";
  }
  std::cout << "+-----------+-----------+-----------+\n";
}

void printone(std::vector<State> const& states, int h)
{
  std::cout << "+";
  for(int w = 1; w <= h/5; ++w) {
    std::cout << "-";
  }
  std::cout << ">\n";
  for (auto& state : states){
    if (state.S <= state.I){
      if (state.I <= state.R) {
        std::cout << "|" << std::setw(state.S/5) << "S" << std::setw((state.I - state.S)/5) << "I" << std::setw((state.R - state.I + state.S)/5) << "R\n"; 
      }
      else if (state.R <= state.S) {
        std::cout << "|" << std::setw(state.R/5) << "R" << std::setw((state.S - state.R)/5) << "S" << std::setw((state.I - state.S + state.R)/5) << "I\n";
      }
      else {
        std::cout << "|" << std::setw(state.S/5) << "S" << std::setw((state.R - state.S)/5) << "R" << std::setw((state.I - state.R + state.S)/5) << "I\n";
      }
    }
    else {
      if (state.S <= state.R) {
        std::cout << "|" << std::setw(state.I/5) << "I" << std::setw((state.S - state.I)/5) << "S" << std::setw((state.R - state.S + state.I)/5) << "R\n"; 
      }
      else if (state.I <= state.R) {
        std::cout << "|" << std::setw(state.I/5) << "I" << std::setw((state.R - state.I)/5) << "R" << std::setw((state.S - state.R + state.I)/5) << "S\n";
      }
      else {
        std::cout << "|" << std::setw(state.R/5) << "R" << std::setw((state.I - state.R)/5) << "I" << std::setw((state.S - state.I + state.R)/5) << "S\n";
      }
    }
    
  }
}



int main(int argc, char* argv[]) {
try {

if (argc!=6) {
  throw std::runtime_error{"5 inputs required"};
}

for (int i=1; i!=4;++i) {
  if (atoi(argv[i])<=0) {
    throw std::runtime_error{"inputs must be positive and different from zero"};
  } 
}

if (std::stod(argv[4])>1 || std::stod(argv[5])>1 || std::stod(argv[4])<=0. || std::stod(argv[5])<=0.) {
  throw std::runtime_error{"gamma and beta must be positive and less than 1"};
}

int S0=std::atoi(argv[1]);
int I0=std::atoi(argv[2]);
int T=std::atoi(argv[3]);
double beta=std::stod(argv[4]);
double gamma=std::stod(argv[5]);

State s0{};
s0.S=S0;
s0.I =I0;
s0.R =0;
s0.b =beta;
s0.g =gamma;
Epidemic covid{s0};
auto const states=covid.evolve(T);
print(states);
if (states.back().I==0) {
  std::cout<<"ciao\n";
}
printone(states, S0+I0);
    
}
catch (std::runtime_error const& e) {
  std::cerr << e.what() << '\n';

} 
}


