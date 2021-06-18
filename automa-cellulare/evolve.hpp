#ifndef EVOLVE_HPP
#define EVOLVE_HPP

#include "sir.hpp"

std::default_random_engine eng{std::random_device{}()};
std::uniform_int_distribution<int> distr{-1, 1};

inline pandemic::Population evolve(pandemic::Population const &current,
                                   situation::State state, bool v_ok,
                                   int v_eff) {
  int const N = current.side();
  int const time = 1 / state.g;
  pandemic::Population next = current;  // costruttore di copia? implicito

  int const R0 = state.b / state.g;
  std::default_random_engine eng{
      std::random_device{}()};  // questo serve subito sotto per parte decimale
                                // R0
  std::uniform_real_distribution<double> dist{0, 1};

  // VACCINATION
  if (v_ok == true) {
    int vax_per_day = N * N * 0.01;

    while (vax_per_day > 0) {
      if (vax_per_day >= N * N - current.infected() - current.recovered() - current.vaccinated()) {
        for (int r = 0; r != N; ++r) {
          for (int c = 0; c != N; ++c) {
            if (current.human(r, c).Is == pandemic::Human::S) {
                next.human(r, c).v = true;
            }
          }
        }
        break;
      }
      std::default_random_engine eng{std::random_device{}()};
      std::uniform_int_distribution<int> pos{0, N};
      int r = pos(eng);
      int c = pos(eng);
      if (current.human(r, c).Is == pandemic::Human::S &&
          current.human(r, c).v == false) {
        next.human(r, c).v = true;
        --vax_per_day;
      }
    }
  }

  // INFECTION
  for (int r = 0; r != N; ++r) {
    for (int c = 0; c != N; ++c) {
      if (current.human(r, c).Is == pandemic::Human::I) {
        auto prob = dist(eng);
        int i_left = 0;
        if (prob <= (state.b / state.g) - R0) {
          i_left = -1;
        }
        int s_left = contacts(current, r, c);

        if (s_left > R0) {
          while (i_left != R0) {
            auto x = distr(eng);
            auto y = distr(eng);
            if (current.human(r + x, c + y).Is == pandemic::Human::S) {
              if (current.human(r + x, c + y).v == true) {
                auto vax_res = dist(eng);
                if (vax_res > v_eff) {
                  next.human(r + x, c + y).Is = pandemic::Human::I;
                }
              } else {
                next.human(r + x, c + y).Is = pandemic::Human::I;
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
            auto x = distr(eng);
            auto y = distr(eng);
            if (current.human(r + x, c + y).Is == pandemic::Human::S) {
              if (current.human(r + x, c + y).v == true) {
                auto vax_res = dist(eng);
                if (vax_res > v_eff) {
                  next.human(r + x, c + y).Is = pandemic::Human::I;
                }
              } else {
                next.human(r + x, c + y).Is = pandemic::Human::I;
              }
              ++i_left;
            } else {
              continue;
            }
          }
        }
        ++next.human(r, c).d;
        if (prob <= (1 / state.g) - time) {
          --next.human(r, c).d;
        }
        if (next.human(r, c).d == time) {
          next.human(r, c).Is = pandemic::Human::R;
        }
      }
    }
  }
  return next;
}  // evolve

#endif