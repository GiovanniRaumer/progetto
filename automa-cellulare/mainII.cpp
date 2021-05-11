#include <chrono>
#include <iostream>
#include <thread>
#include <random>

#include "sirII.hpp"

void print(std::ostream& os, pandemic::Population const& population) {
  // constexpr auto clear = "\033[2J";
  const auto N = population.side();

  // os << clear;
  os << '+' << std::string(N, '-') << "+\n";
  for (int r = 0; r != N; ++r) {
    os << '|';
    for (int c = 0; c != N; ++c) {
      if (population.human(r, c) == pandemic::Human::Susceptible) { os<<' ';}
      else if (population.human(r, c) == pandemic::Human::Infected) { os<<'X';}
      else { os<<' ';}

    }
    os << '|' << '\n';
  }
  os << '+' << std::string(N, '-') << "+\n";
}

int main (int argc, char* argv[]) {
  try {
    if (argc != 6) {
      throw std::runtime_error{"5 inputs required"};
    }

    for (int i = 1; i != 4; ++i) {
      if (atoi(argv[i]) <= 0) {
        throw std::runtime_error{
            "inputs must be positive and different from zero"};
      }
    }

    if (std::stod(argv[4]) > 1 || std::stod(argv[5]) > 1 ||
        std::stod(argv[4]) <= 0. || std::stod(argv[5]) <= 0.) {
      throw std::runtime_error{
          "gamma and beta must be positive and less than 1"};
    }

    int I0 = std::atoi(argv[2]);
    //int T = std::atoi(argv[3]);
    //double beta = std::stod(argv[4]);
    //double gamma = std::stod(argv[5]);

    int const N = std::atoi(argv[1]);
    pandemic::Population world(N);

    std::default_random_engine eng{std::random_device{}()};
    std::uniform_int_distribution<int> dist{0, world.side() - 1};

    for (int i=0; i!=I0; ++i) {
        int x= dist(eng);
        int y=dist(eng);
        world.human(x,y)=pandemic::Human::Infected;
    }
    print(std::cout, world);

    

    /*for (int i = 0; i != 4; ++i) {
      population = evolve(population);
      print(std::cout, population);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }*/
  } catch (std::runtime_error const& e) {
    std::cerr << e.what() << '\n';
  }
}