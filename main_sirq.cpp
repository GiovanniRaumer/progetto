#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "sirq.hpp"

void print(std::vector<State> const& states) {
  std::cout << "+-----------+-----------+-----------+\n"
               "|     S     |     I     |     R     |\n"
               "+-----------+-----------+-----------+\n"
            << std::fixed;
  for (auto& state : states) {
    std::cout << "| " << std::setw(9) << state.S << " | " << std::setw(9)
              << state.I << " | " << std::setw(9) << state.R << " |\n";
  }
  std::cout << "+-----------+-----------+-----------+\n";
}

void printone(std::vector<State> const& states, int h, int q) {
  std::cout << "+";
  for (int w = 1; w <= h / q; ++w) {
    std::cout << "-";
  }
  std::cout << ">\n";
  for (auto& state : states) {
    if (state.S <= state.I) {
      if (state.I <= state.R) {
        std::cout << "|" << std::setw(1 + state.S / q) << "S"
                  << std::setw((state.I - state.S) / q) << "I"
                  << std::setw((state.R - state.I) / q) << "R\n";
      } else if (state.R <= state.S) {
        std::cout << "|" << std::setw(1 + state.R / q) << "R"
                  << std::setw((state.S - state.R) / q) << "S"
                  << std::setw((state.I - state.S) / q) << "I\n";
      } else {
        std::cout << "|" << std::setw(1 + state.S / q) << "S"
                  << std::setw((state.R - state.S) / q) << "R"
                  << std::setw((state.I - state.R) / q) << "I\n";
      }
    } else {
      if (state.S <= state.R) {
        std::cout << "|" << std::setw(1 + state.I / q) << "I"
                  << std::setw((state.S - state.I) / q) << "S"
                  << std::setw((state.R - state.S) / q) << "R\n";
      } else if (state.I <= state.R) {
        std::cout << "|" << std::setw(1 + state.I / q) << "I"
                  << std::setw((state.R - state.I) / q) << "R"
                  << std::setw((state.S - state.R) / q) << "S\n";
      } else {
        std::cout << "|" << std::setw(1 + state.R / q) << "R"
                  << std::setw((state.I - state.R) / q) << "I"
                  << std::setw((state.S - state.I) / q) << "S\n";
      }
    }
  }
}

int main(int argc, char* argv[]) {
  try {
    if (argc != 6 || argc != 8) {
      throw std::runtime_error{"5 or 7 inputs required"};
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

    double q_start = 1.;
    double q_eff = 1.;
    if (argc == 8) {
      if (std::stod(argv[6]) <= 0 || std::stod(argv[6]) >= 1 ||
          std::stod(argv[7]) <= 0 || std::stod(argv[7]) >= 1) {
        throw std::runtime_error{
            "the turning point of quarantine and its efficency must be "
            "positive (not 0) and less than 1"};
      }
    } else {
      q_start = std::stod(argv[6]);
      q_eff = std::stod(argv[7]);
    }

  int S0 = std::atoi(argv[1]);
  int I0 = std::atoi(argv[2]);
  int T = std::atoi(argv[3]);
  double beta = std::stod(argv[4]);
  double gamma = std::stod(argv[5]);

  State s0{};
  s0.S = S0;
  s0.I = I0;
  s0.R = 0;
  s0.b = beta;
  s0.g = gamma;
  Epidemic covid{s0};
  auto const states = covid.evolve(T, q_start, q_eff);
  print(states);
  if (states.back().I == 0) {
    std::cout << "end of epidemic\n";
  }

  int q = (S0 + I0) / 150;
  if (S0 + I0 <= 150) {
    q = 1;
  }
  printone(states, S0 + I0, q);
}
catch (std::runtime_error const& e) {
  std::cerr << e.what() << "INSTRUCTIONS:" << '\n'
              << "- mandatory - \n"
              << "1st input --> initial susceptible subjects \n"
              << "2nd input --> initial infected subjects \n"
              << "3rd input --> duration of simulation (days) \n"
              << "4th input --> beta \n"
              << "5th input --> gamma \n"
              << "- optional - \n"
              << "6th input --> population percentage to start quarantine \n"
              << "7th input --> quarantine effectiveness (new-beta / previous-beta) \n"
              << '\n';
}
}