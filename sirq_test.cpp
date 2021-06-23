#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "sirq.hpp"

TEST_CASE("Testing equations") {
  State initial_state;
  initial_state.S = 1000;
  initial_state.I = 40;
  initial_state.b = 0.7;
  initial_state.g = 0.4;

  int const N = initial_state.S + initial_state.I + initial_state.R;

  int T = 200;

  Epidemic ep(initial_state);

  SUBCASE("No lockdown") {
    int q_start = 1.;
    int q_eff = 1.;
    auto const evolution = ep.evolve(T, q_start, q_eff);
    auto const last_state = evolution.back();
    // Testing conservation of total population
    CHECK(last_state.S + last_state.I + last_state.R == N);
    // Testing end of epidemic --> Infected = 0
    CHECK(last_state.I == 0);
    // Testing conservation of beta
    CHECK(initial_state.b == last_state.b);
  }

  SUBCASE("Lockdown") {
    int q_start = 0.1;
    int q_eff = 0.6;
    auto const evolution = ep.evolve(T, q_start, q_eff);
    auto const last_state = evolution.back();
    // Testing conservation of total population
    CHECK(last_state.S + last_state.I + last_state.R == N);
    // Testing end of epidemic --> Infected = 0
    CHECK(last_state.I == 0);
    // Testing non-conservation of beta
    CHECK(initial_state.b > last_state.b);
  }
}
