#include "sir.hpp"

std::default_random_engine eng{std::random_device{}()};
std::uniform_int_distribution<int> distr{-1, 1};

inline pandemic::Population evolve(pandemic::Population const &current, situation::State state, char def, int p1, int p2) {
  if (p1 = 0) {
      
  }
  int const N = current.side();
  int const time = 1 / state.g;
  pandemic::Population next = current;  // costruttore di copia? implicito

  int const R0 = state.b / state.g;
  std::default_random_engine eng{std::random_device{}()};  // questo serve subito sotto per parte decimale R0
  std::uniform_real_distribution<double> dist{0, 1};

  for (int r = 0; r != N; ++r) {
    for (int c = 0; c != N; ++c) {
      if (current.human(r, c).Is == pandemic::Human::I) {
        auto prob = dist(eng);
        int i_left = 0;
        if (prob <= (state.b / state.g) - R0) {
          i_left = -1;
        } else {
          i_left = 0;
        }
        int s_left = contacts(current, r, c);

        if (s_left > R0) {
          while (i_left != R0) {
            auto x = distr(eng);
            auto y = distr(eng);
            if (current.human(r + x, c + y).Is == pandemic::Human::S) {
              next.human(r + x, c + y).Is = pandemic::Human::I;
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
              next.human(r + x, c + y).Is = pandemic::Human::I;
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
  while (p1 >= 0) {
      --p1;
  }
  return next;
} //evolve