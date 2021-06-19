#include <chrono>
#include <string>  //per std::stod
#include <thread>

#include "evolve.hpp"
#include "sir.hpp"

void print(pandemic::Population const &population) {
  const auto N = population.side();

  std::cout << "\033[2J";
  std::cout << " day " << population.day << " | S : "
            << N * N - population.infected() - population.recovered() << " | "
            << "I : " << population.infected() << " | "
            << "R : " << population.recovered() << " | "
            << "V : " << population.vaccinated() << '\n';

  std::cout << '+' << std::string(N, '-') << "+\n";
  for (int r = 0; r != N; ++r) {
    std::cout << '|';
    for (int c = 0; c != N; ++c) {
      switch (population.human(r, c).Is) {
        case pandemic::Human::I:
          std::cout << "\033[31mX\033[0m";
          break;
        case pandemic::Human::R:
          std::cout << "\033[33mR\033[0m";
          break;
        default:
          if (population.human(r, c).v == true) {
            std::cout << "\033[35mV\033[0m";
          } else {
            std::cout << "\033[36mO\033[0m";
          }
          break;
      }
    }
    std::cout << '|' << '\n';
  }
  std::cout << '+' << std::string(N, '-') << "+\n";
}

int main(int argc, char *argv[]) {
  try {
    if (argc != 6 && argc != 9) {
      throw std::runtime_error{"5 or 8 inputs required \n"};
    }

    if (argc == 6 || argc == 9) {
      for (int i = 1; i != 4; ++i) {
        if (atoi(argv[i]) <= 0) {
          throw std::runtime_error{
              "inputs must be positive and different from zero \n"};
        }
      }

      for (int i = 4; i != 6; ++i) {
        if (std::stod(argv[i]) <= 0 || std::stod(argv[i]) > 1) {
          throw std::runtime_error{
              "gamma and beta must be positive and less than 1 \n"};
        }
      }

      if (std::atoi(argv[2]) > std::atoi(argv[1]) * std::atoi(argv[1])) {
        throw std::runtime_error{"infected must be less than/equal to total \n"};
      }
    }

    int v_begin = 0;
    double v_eff = 0.;
    if (argc == 9) {
      if (*argv[6] != 'v' && *argv[6] != 'V') {
        throw std::runtime_error{"6th input must be \"v (V)\" \n"};
      } else {
        if (std::atoi(argv[7]) >= std::atoi(argv[3]) ||
            std::atoi(argv[7]) < 1 ||
            (std::stod(argv[7]) - std::atoi(argv[7])) != 0) {
          throw std::runtime_error{
              "the vaccination period should be integer and start after the "
              "first iteration and before the last one \n"};
        }
        if (std::atoi(argv[8]) < 0 || std::atoi(argv[8]) > 100 ||
            (std::stod(argv[8]) - std::atoi(argv[8])) != 0) {
          throw std::runtime_error{
              "vaccine effectiveness must be positive and less than 1 \n"};
        }
      }
      v_begin = std::atoi(argv[7]);
      v_eff = std::atoi(argv[8]);
    }

    int const N = std::atoi(argv[1]);
    int const I0 = std::atoi(argv[2]);
    int const T = std::atoi(argv[3]);
    double beta = std::stod(argv[4]);
    double gamma = std::stod(argv[5]);
    bool v_ok = false;

    pandemic::Population population(N, I0);
    situation::State state;
    state.S = N * N - I0;
    state.I = I0;
    state.R = 0;
    state.b = beta;
    state.g = gamma;

    std::cout << v_begin << " " << v_eff << '\n';

    for (int i = 0; i != T; ++i) {
      print(population);
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      if (population.infected() == 0) {
        break;
      }

      ++population.day;

      if (v_eff != 0 && (i + 1) == v_begin) {
        v_ok = true;
      }
      state = situation::evolve(state);
      //    population = evolve(population, state);
      population = evolve(population, state, v_ok, v_eff);
    }
  } catch (std::runtime_error const &e) {
    std::cerr << e.what() << "INSTRUCTIONS:" << '\n'
              << "- necessary - \n"
              << "1st input --> grid side \n"
              << "2nd input --> initial infected subjects \n"
              << "3rd input --> duration of simulation (days) \n"
              << "4th input --> beta \n"
              << "5th input --> gamma \n"
              << "- optional - \n"
              << "6th input --> v or V for vaccine \n"
              << "7th input --> day of vaccination beginning \n"
              << "8th input --> vaccine effectiveness \n"
              << '\n';
  }
}