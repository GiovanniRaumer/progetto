#ifndef SEIQR_HPP
#define SEIQR_HPP

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector> 

struct State { 
  int S;
  int E;
  int I;
  int Q;
  int R;
  double a; //E-->I
  double b; //infection rate
  double g; //I-->R
  double d; //natural mortality rate
  double k; //E-->R
  double l; //I-->Q
  double e; //Q-->R
};

class Epidemic {
  State e_initial_state;

 public:
  Epidemic(State initial_state) : e_initial_state{initial_state} {}

  std::vector<State> evolve(int T) {
    std::vector<State> result{e_initial_state};
    int const N = result.back().S + result.back().E + result.back().I + result.back().Q + result.back().R;
    for (int t = 1; t != T; ++t) {
      State const& prev = result.back();
      State next;
      next.S = round(prev.S - prev.b * prev.S * (prev.I + prev.E) / N - prev.d * prev.S);
      next.E = round(prev.E + prev.b * prev.S * (prev.E + prev.I) / N -(prev.d + prev.a + prev.k) * prev.E);
      next.I = round(prev.I + prev.a * prev.E - (prev.d + prev.g + prev.l) * prev.I);
      next.Q = round(prev.Q + prev.l * prev.I - (prev.d + prev.e) * prev.Q);
      next.R = N-next.S-next.E-next.I-next.Q;
      next.a = prev.a;
      next.b = prev.b;
      next.g = prev.g;
      next.d = prev.d;
      next.k = prev.k;
      next.l = prev.l;
      next.e = prev.e;

      /*if (next.S==prev.S && next.E==prev.E && next.I==prev.I && next.Q==prev.Q && next.R==prev.R) {
        next.I=prev.I-1;
        next.R=prev.R+1;
      }*/
      assert(N==next.S+next.E+next.I+next.Q+next.R);
      result.push_back(next);
      if (next.I==0) {
        break;
      }
    }
    return result;
  }
};

#endif