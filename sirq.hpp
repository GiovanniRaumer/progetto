#ifndef SIRQ_HPP
#define SIRQ_HPP

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector> 

struct State { //da aggiungere E e Q
  int S;
  int I;
  int R;
  double b;
  double g;
};

class Epidemic {
  State e_initial_state;

 public:
  Epidemic(State initial_state) : e_initial_state{initial_state} {}

  std::vector<State> evolve(int T) {
    std::vector<State> result{e_initial_state};
    int const N = result.back().S + result.back().I + result.back().R;
    for (int t = 1; t != T; ++t) {
      State const& prev = result.back();
      State next;
      if (result.back().I <= 0.1*N){
        next.b = prev.b;
      }
      else {
        next.b = prev.b * (2/3);
      }
      next.S = round(prev.S - next.b * prev.S * prev.I / N);
      next.I = round(prev.I + next.b * prev.S * prev.I / N - prev.g * prev.I);
      next.R = N-next.S-next.I;
      next.g = prev.g;
      if (next.S==prev.S && next.I==prev.I && next.R==prev.R) {
        next.I=prev.I-1;
        next.R=prev.R+1;
      }
      assert(N==next.S+next.I+next.R);
      result.push_back(next);
      if (next.I==0) {
        break;
      }
    }
    return result;
  }
};

#endif