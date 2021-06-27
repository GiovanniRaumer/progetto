#ifndef SIR_HPP
#define SIR_HPP

#include <cassert>
#include <random>
#include <vector>

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
  Population(int N, int I0);

  int side() const { return w_side; }

  Human const &human(int r, int c) const noexcept;

  Human &human(int r, int c) noexcept;

  int infected() const;

  int recovered() const;

  int vaccinated() const;

  int S_not_vax() const;
};

// CONTACTS
inline int contacts(Population const &population, int r, int c) {
  int counter = 0;
  for (int i : {-1, 0, 1}) {
    for (int j : {-1, 0, 1}) {
      if (r + i == -1 || r + i == population.side() || c + j == -1 ||
          c + j == population.side()) {
        continue;
      } else if (population.human(r + i, c + j).Is == pandemic::Human::S) {
        ++counter;
      }
    }
  }
  return counter;
}

// EVOLVE
inline Population evolve(Population const &current, double beta, double gamma,
                         int i, int v_begin, double v_eff) {
  std::default_random_engine eng{std::random_device{}()};
  std::uniform_int_distribution<int> int_dist{-1, 1};
  std::uniform_real_distribution<double> real_dist{0, 1};

  int const N = current.side();
  int const recovery_time = 1 / gamma;
  Population next = current;

  int const R0 = beta / gamma;

  // VACCINATION
  if (v_eff != 0. && (i + 2) >= v_begin) {
    int vax_per_day = N * N * 0.01;

    std::uniform_int_distribution<int> position{0, N - 1};
    int r = position(eng);
    int c = position(eng);

    if (vax_per_day >= current.S_not_vax()) {
      for (int r = 0; r != N; ++r) {
        for (int c = 0; c != N; ++c) {
          if (current.human(r, c).Is == Human::S) {
            next.human(r, c).v = true;
          }
        }
      }
    } else {
      for (; vax_per_day > 0; r = position(eng), c = position(eng)) {
        if (current.human(r, c).Is == Human::S && next.human(r, c).v == false) {
          next.human(r, c).v = true;
          --vax_per_day;
        }
      }
    }
  }

  // INFECTION
  for (int r = 0; r != N; ++r) {
    for (int c = 0; c != N; ++c) {
      if (current.human(r, c).Is == Human::I) {
        auto prob = real_dist(eng);

        int i_left = 0;
        if (prob <= (beta / gamma) - R0) {
          i_left = -1;
        }

        int s_left = contacts(current, r, c);

        if (s_left > R0) {
          while (i_left != R0) {
            auto x = int_dist(eng);
            auto y = int_dist(eng);

            if (current.human(r + x, c + y).Is == Human::S) {
              if (current.human(r + x, c + y).v == true) {
                auto vax_res = real_dist(eng);

                if (vax_res > v_eff) {
                  next.human(r + x, c + y).Is = Human::I;
                }
              } else {
                next.human(r + x, c + y).Is = Human::I;
              }
              ++i_left;
            } else {
              continue;
            }
          }
        }
        if (s_left <= R0) {
          i_left = R0 - s_left;
          while (i_left != R0) {
            auto x = int_dist(eng);
            auto y = int_dist(eng);

            if (current.human(r + x, c + y).Is == Human::S) {
              if (current.human(r + x, c + y).v == true) {
                auto vax_res = real_dist(eng);

                if (vax_res > v_eff) {
                  next.human(r + x, c + y).Is = Human::I;
                }
              } else {
                next.human(r + x, c + y).Is = Human::I;
              }
              ++i_left;
            } else {
              continue;
            }
          }
        }
        ++next.human(r, c).days_I;
        if (prob <= (1 / gamma) - recovery_time) {
          --next.human(r, c).days_I;
        }
        if (next.human(r, c).days_I == recovery_time) {
          next.human(r, c).Is = Human::R;
        }
      }
    }
  }
  return next;
}

}  // namespace pandemic

#endif