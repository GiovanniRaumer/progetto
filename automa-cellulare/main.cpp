#include <chrono>
#include <string>  //per std::stod
#include <thread>

#include "sir.hpp"

void print(pandemic::Population const &population) {
  const auto N = population.side();

  std::cout << "\033[2J";
  std::cout << "| S : "
            << N * N - population.infected() - population.recovered() << " | "
            << "I : " << population.infected() << " | "
            << "R : " << population.recovered() << " | " << '\n';
  std::cout << '+' << std::string(N, '-') << "+\n";
  for (int r = 0; r != N; ++r) {
    std::cout << '|';
    for (int c = 0; c != N; ++c) {
      switch (population.human(r, c).Is) {
        case pandemic::Human::I:
          std::cout << "\033[31mX\033[0m";
          break;
        case pandemic::Human::R:
          std::cout << "\033[33mO\033[0m";
          break;
        default:
          std::cout << "\033[36mO\033[0m";
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
      throw std::runtime_error{"5 or 8 inputs required"};
    }

    if (argc == 6 || argc == 9) {
      for (int i = 1; i != 4; ++i) {
        if (atoi(argv[i]) <= 0) {
          throw std::runtime_error{
              "inputs must be positive and different from zero"};
        }
      }

      for (int i = 4; i != 6; ++i) {
        if (std::stod(argv[i]) <= 0 || std::stod(argv[i]) > 1) {
          throw std::runtime_error{
              "gamma and beta must be positive and less than 1"};
        }
      }

      if (std::atoi(argv[2]) > std::atoi(argv[1]) * std::atoi(argv[1])) {
        throw std::runtime_error{"infected must be less than/equal to total"};
      }
    }

    if (argc == 9) {
      if (*argv[6] != 'v' && *argv[6] != 'V' && *argv[6] != 'q' &&
          *argv[6] != 'Q') {
        throw std::runtime_error{"6th input must be \"v (V)\" or \"q (Q)\""};
      }

      if (*argv[6] == 'v' || *argv[6] == 'V') {
        if (std::atoi(argv[7]) >= std::atoi(argv[3]) ||
            std::atoi(argv[7]) < 0 ||
            (std::stod(argv[7]) - std::atoi(argv[7])) != 0) {
          throw std::runtime_error{
              "the vaccination period should be integer and start after the first "
              "iteration and before the last one"};
        }
        if (std::atoi(argv[8]) < 0 || std::atoi(argv[8]) > 100 || (std::stod(argv[8]) - std::atoi(argv[8])) != 0) {
          throw std::runtime_error{
              "vaccine effectiveness must be an integer between 0 and 100"};
        }
        int const v_begin = std::atoi(argv[7]);
        double const v_eff = std::stod(argv[8]);
      }

      if (*argv[6] == 'q' || *argv[6] == 'Q') {
        if (std::atoi(argv[7]) >= std::atoi(argv[3]) ||
            std::atoi(argv[7]) < 0 ||
            (std::stod(argv[7]) - std::atoi(argv[7])) != 0) {
          throw std::runtime_error{
              "the quarantine period should be integer and start after the first "
              "iteration and before the last one"};
        }
        if (std::atoi(argv[8]) < 0 ||
            (std::stod(argv[8]) - std::atoi(argv[8])) != 0) {
          throw std::runtime_error{
              "the quarantine period must be a positive integer number"};
        }
        int const q_begin = std::atoi(argv[7]);
        int const q_period = std::atoi(argv[8]);
      }
    }

    int const N = std::atoi(argv[1]);
    int const I0 = std::atoi(argv[2]);
    int const T = std::atoi(argv[3]);
    double beta = std::stod(argv[4]);
    double gamma = std::stod(argv[5]);

    pandemic::Population population(N, I0);
    situation::State state;
    state.S = N * N - I0;
    state.I = I0;
    state.R = 0;
    state.b = beta;
    state.g = gamma;

    for (int i = 0; i != T; ++i) {
      print(population);
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      if (population.infected() == 0) {
        break;
      }
      state = situation::evolve(state);
      population = evolve(population, state);
    }
  } catch (std::runtime_error const &e) {
    std::cerr << e.what() << '\n';
  }
}