#include <iomanip>
#include <iostream>
#include "seiqr.hpp"
#include <stdexcept>


void print(std::vector<State> const& states)
{

  std::cout << "+-----------+-----------+-----------+-----------+-----------+\n"
               "|     S     |     E     |     I     |     Q     |     R     |\n"
               "+-----------+-----------+-----------+-----------+-----------+\n"
            << std::fixed;
  for (auto& state : states) {
    std::cout << "| " << std::setw(9) << state.S << " | " << std::setw(9) << state.E << " | "
              << std::setw(9) << state.I << " | " << std::setw(9) << state.Q << " | " 
              << std::setw(9) << state.R <<" |\n";
  }
  std::cout << "+-----------+-----------+-----------+-----------+-----------+\n";
}



int main(int argc, char* argv[]) {
try {

if (argc!=12) {
  throw std::runtime_error{"11 inputs required"};
}

for (int i=1; i!=4;++i) {
  if (atoi(argv[i])<=0) {
    throw std::runtime_error{"inputs must be positive and different from zero"};
  } 
}

/*for (int j=5; j!=11;++j) {
  if (std::stod(argv[j]) <= 0. || std::stod(argv[j]) > 1) {
    throw std::runtime_error{"all coefficients must be positive and less than 1"};
  } 
}*/

int S0=std::atoi(argv[1]);
int E0=std::atoi(argv[2]);
int I0=std::atoi(argv[3]);
int T=std::atoi(argv[4]);
double alfa=std::stod(argv[5]);
double beta=std::stod(argv[6]);
double gamma=std::stod(argv[7]);
double d=std::stod(argv[8]);
double k=std::stod(argv[9]);
double delta=std::stod(argv[10]);
double epsilon=std::stod(argv[11]);

State s0{};
s0.S=S0;
s0.E=E0;
s0.I=I0; 
s0.R=0;
s0.a=alfa;
s0.b=beta;
s0.g=gamma;
s0.d=d;
s0.k=k;
s0.l=delta;
s0.e=epsilon;

Epidemic covid{s0};
auto const states=covid.evolve(T);
print(states);
if (states.back().I==0) {
  std::cout<<"ciao\n";
}
    
}
catch (std::runtime_error const& e) {
  std::cerr << e.what() << '\n';

} 
}