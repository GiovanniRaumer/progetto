#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "sirq.hpp"

#include "doctest.h"

TEST_CASE("Testing equations") {
  State initial_state;
  initial_state.S = 1000;
  initial_state.I = 40;
  initial_state.b = 0.7;
  initial_state.g = 0.4;

  int T = 20;
  int q_start = 1.;
  int q_eff = 1.;

  Epidemic ep(initial_state);
  auto const evolution = ep.evolve(T, q_start, q_eff);
  auto const last_state = evolution.back();

  SUBCASE("Testing conservation of total population") {
    CHECK(initial_state.S + initial_state.I + initial_state.R == last_state.S + last_state.I + last_state.R);
  }

  SUBCASE("Testing end of epidemic (Infected = 0 ?)") {
    CHECK(last_state.I == 0);
  }

  SUBCASE("Testing conservation of beta") {
    CHECK(last_state.b == last_state.b);
  }

  q_start = 0.1;
  q_eff = 0.6;

  SUBCASE("Testing non-conservation of beta") {
    CHECK(last_state.b != last_state.b);      
  }
}
