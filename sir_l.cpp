#include "sir_l.hpp"

std::vector<State> Epidemic::evolve(int T, double l_start, double l_eff) {
  std::vector<State> result{e_initial_state};
  int const N = result.back().S + result.back().I + result.back().R;
  for (int t = 1; t != T; ++t) {
    State const& prev = result.back();
    State next;
    if (result.back().I <= l_start * N) {
      next.b = prev.b;
    } else {
      next.b = prev.b * l_eff;
    }
    next.S = std::round(prev.S - next.b * prev.S * prev.I / N);
    next.I =
        std::round(prev.I + next.b * prev.S * prev.I / N - prev.g * prev.I);
    next.R = N - next.S - next.I;
    next.g = prev.g;
    if (next.S == prev.S && next.I == prev.I && next.R == prev.R) {
      next.I = prev.I - 1;
      next.R = prev.R + 1;
    }
    assert(N == next.S + next.I + next.R);
    result.push_back(next);
    if (next.I == 0) {
      break;
    }
  }
  return result;
}