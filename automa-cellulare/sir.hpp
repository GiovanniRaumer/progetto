#ifndef SIR_HPP
#define SIR_HPP

#include <vector>
#include <random>
#include <cassert>

namespace pandemic {

struct Human {
  enum IS : char {
    S,  // Susceptible
    I,  // Infected
    R   // Recovered
  };

  IS Is;
  int days_I = 0;  // days_Infected
  bool v = false;  // is vaccinated?
};

class Population {
  using Row = std::vector<Human>;
  using Grid = std::vector<Row>;
  int w_side;
  Grid w_grid;
  int first_I;

 public:
  Population(int N, int I0) : w_side(N), w_grid(N, Row(N)), first_I{I0} {
    assert(w_side > 0 && first_I > 0);

    for (int r = 0; r != N; ++r) {
      for (int c = 0; c != N; ++c) {
        w_grid[r][c].Is = Human::S;
      }
    }

    std::default_random_engine eng{std::random_device{}()};
    std::uniform_int_distribution<int> dist{0, w_side - 1};
    int r = dist(eng);
    int c = dist(eng);

    for (int i = 0; i != first_I; r = dist(eng), c = dist(eng)) {
      if (w_grid[r][c].Is == Human::S) {
        w_grid[r][c].Is = Human::I;
        ++i;
      }
    }
  }
  
  int side() const { return w_side; }

  Human const &human(int r, int c) const noexcept {
    auto const i = (r + w_side) % w_side;
    auto const j = (c + w_side) % w_side;

    assert(i >= 0 && i < w_side && j >= 0 && i < w_side);
    return w_grid[i][j];
  }

  Human &human(int r, int c) noexcept {
    auto const i = (r + w_side) % w_side;
    auto const j = (c + w_side) % w_side;

    assert(i >= 0 && i < w_side && j >= 0 && i < w_side);
    return w_grid[i][j];
  }

  int infected() const {
    int infected = 0;
    for (int r = 0; r != w_side; ++r) {
      for (int c = 0; c != w_side; ++c) {
        if (w_grid[r][c].Is == Human::I) {
          ++infected;
        }
      }
    }
    return infected;
  }

  int recovered() const {
    int recovered = 0;
    for (int r = 0; r != w_side; ++r) {
      for (int c = 0; c != w_side; ++c) {
        if (w_grid[r][c].Is == Human::R) {
          ++recovered;
        }
      }
    }
    return recovered;
  }

  int vaccinated() const {
    int vaccinated = 0;
    for (int r = 0; r != w_side; ++r) {
      for (int c = 0; c != w_side; ++c) {
        if (w_grid[r][c].v == true) {
          ++vaccinated;
        }
      }
    }
    return vaccinated;
  }

  int S_not_vax() const {
    int notvax = 0;
    for (int r = 0; r != w_side; ++r) {
      for (int c = 0; c != w_side; ++c) {
        if (w_grid[r][c].v == false && w_grid[r][c].Is == Human::S) {
          ++notvax;
        }
      }
    }
    return notvax;
  }
};

inline int contacts(Population const &population, int r, int c) {
  int counter = 0;
  for (int i : {-1, 0, 1}) {
    for (int j : {-1, 0, 1}) {
      if (r + i == -1 || r + i == population.side() || c + j == -1 ||
          c + j == population.side()) {
        continue;
      } else if (population.human(r + i, c + j).Is == Human::S) {
        ++counter;
      }
    }
  }
  return counter;
}

}  // namespace pandemic

#endif