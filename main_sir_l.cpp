#include "sir_l.hpp"

#include <iomanip>
#include <stdexcept>

void print(std::vector<State> const& states) {
  std::cout << "+-----------+-----------+-----------+-----------+\n"
               "|    day    |     S     |     I     |     R     |\n"
               "+-----------+-----------+-----------+-----------+\n"
            << std::fixed;
  int T = states.size();
  for (int t = 0; t != T; ++t) {
    std::cout << "| " << std::setw(9) << t + 1 << " | " << std::setw(9) << states[t].S << " | " << std::setw(9)
              << states[t].I << " | " << std::setw(9) << states[t].R << " |\n";
  }
  std::cout << "+-----------+-----------+-----------+-----------+\n";
}

void graph(std::vector<State> const& states, int total, int scale) {
  std::cout << "+";
  for (int y_axis = 1; y_axis <= total / scale; ++y_axis) {
    std::cout << "-";
  }
  std::cout << ">\n";
  for (auto& state : states) {
    if (state.S <= state.I) {
      if (state.I <= state.R) {
        std::cout << "|" << std::setw(1 + state.S / scale) << "S"
                  << std::setw((state.I - state.S) / scale) << "I"
                  << std::setw((state.R - state.I) / scale) << "R\n";
      } else if (state.R <= state.S) {
        std::cout << "|" << std::setw(1 + state.R / scale) << "R"
                  << std::setw((state.S - state.R) / scale) << "S"
                  << std::setw((state.I - state.S) / scale) << "I\n";
      } else {
        std::cout << "|" << std::setw(1 + state.S / scale) << "S"
                  << std::setw((state.R - state.S) / scale) << "R"
                  << std::setw((state.I - state.R) / scale) << "I\n";
      }
    } else {
      if (state.S <= state.R) {
        std::cout << "|" << std::setw(1 + state.I / scale) << "I"
                  << std::setw((state.S - state.I) / scale) << "S"
                  << std::setw((state.R - state.S) / scale) << "R\n";
      } else if (state.I <= state.R) {
        std::cout << "|" << std::setw(1 + state.I / scale) << "I"
                  << std::setw((state.R - state.I) / scale) << "R"
                  << std::setw((state.S - state.R) / scale) << "S\n";
      } else {
        std::cout << "|" << std::setw(1 + state.R / scale) << "R"
                  << std::setw((state.I - state.R) / scale) << "I"
                  << std::setw((state.S - state.I) / scale) << "S\n";
      }
    }
  }
}

int main(int argc, char* argv[]) {
  try {
    if (argc != 6 && argc != 8) {
      throw std::runtime_error{"5 or 7 inputs required \n"};
    }

    for (int i = 1; i != 4; ++i) {
      if (atoi(argv[i]) <= 0) {
        throw std::runtime_error{
            "inputs must be positive and different from zero \n"};
      }
    }

    if (std::stod(argv[4]) > 1 || std::stod(argv[5]) > 1 ||
        std::stod(argv[4]) <= 0. || std::stod(argv[5]) <= 0.) {
      throw std::runtime_error{
          "gamma and beta must be positive and less than 1 \n"};
    }

    double q_start = 1.;
    double q_eff = 1.;
    if (argc == 8) {
      if (std::stod(argv[6]) <= 0 || std::stod(argv[6]) >= 1 ||
          std::stod(argv[7]) <= 0 || std::stod(argv[7]) >= 1) {
        throw std::runtime_error{
            "the turning point of quarantine and its efficency must be "
            "positive (not 0) and less than 1 \n"};
      } else {
        q_start = std::stod(argv[6]);
        q_eff = std::stod(argv[7]);
      }
    }

    int T = std::atoi(argv[3]);

    State s0{};
    s0.S = std::atoi(argv[1]);
    s0.I = std::atoi(argv[2]);
    s0.R = 0;
    s0.b = std::stod(argv[4]);
    s0.g = std::stod(argv[5]);

    Epidemic plague{s0};
    auto const states = plague.evolve(T, q_start, q_eff);

    print(states);

    if (states.back().I == 0) {
      std::cout << "end of epidemic\n";
    }

    int scale = (s0.S + s0.I) / 150;
    if (s0.S + s0.I <= 150) {
      scale = 1;
    }
    graph(states, s0.S + s0.I, scale);
    
  } catch (std::runtime_error const& e) {
    std::cerr << e.what() << "INSTRUCTIONS:" << '\n'
              << "- mandatory - \n"
              << "1st input --> initial susceptible subjects \n"
              << "2nd input --> initial infected subjects \n"
              << "3rd input --> duration of simulation (days) \n"
              << "4th input --> beta \n"
              << "5th input --> gamma \n"
              << "- optional - \n"
              << "6th input --> population percentage to start quarantine \n"
              << "7th input --> quarantine effectiveness (new-beta / "
                 "previous-beta) \n"
              << '\n';
  }
}