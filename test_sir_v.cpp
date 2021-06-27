#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "sir_v.hpp"

TEST_CASE("Testing immunity") {
  pandemic::Population small(2, 4);
  double beta = 0.7;
  double gamma = 0.4;
  SUBCASE("No vaccination") {
    int i = 0;
    int v_begin = 0;
    double v_eff = 0;
    small.human(1, 1).Is = pandemic::Human::R;
    CHECK(small.human(1, 1).Is == pandemic::Human::R);

    auto const next = evolve(small, beta, gamma, i, v_begin, v_eff);
    CHECK(next.human(1, 1).Is == small.human(1, 1).Is);
  }
  SUBCASE("Vaccination") {
    int i = 3;
    int v_begin = 5;
    double v_eff = 0.94;
    small.human(1, 1).Is = pandemic::Human::R;
    CHECK(small.human(1, 1).Is == pandemic::Human::R);

    auto const next = evolve(small, beta, gamma, i, v_begin, v_eff);
    CHECK(next.human(1, 1).Is == small.human(1, 1).Is);
  }
}

TEST_CASE("Testing increasing of vaccinated subjects") {
  pandemic::Population med(10, 10);
  double beta = 0.7;
  double gamma = 0.4;
  int T = 10;
  int v_begin = 2;
  double v_eff = 0.94;
  pandemic::Population next = med;
  for (int i = 0; i != T; ++i) {
    next = evolve(next, beta, gamma, i, v_begin, v_eff);
  }
  CHECK(next.vaccinated() > med.vaccinated());
}

TEST_CASE("Testing increasing of recovered subjects") {
  pandemic::Population med(10, 10);
  double beta = 0.7;
  double gamma = 0.4;
  SUBCASE("No vaccination") {
    int T = 10;
    int v_begin = 0;
    double v_eff = 0;
    pandemic::Population next = med;
    for (int i = 0; i != T; ++i) {
      next = evolve(next, beta, gamma, i, v_begin, v_eff);
    }
    CHECK(next.recovered() > med.recovered());
  }
  SUBCASE("Vaccination") {
    int T = 10;
    int v_begin = 2;
    double v_eff = 0.94;
    pandemic::Population next = med;
    for (int i = 0; i != T; ++i) {
      next = evolve(next, beta, gamma, i, v_begin, v_eff);
    }
    CHECK(next.recovered() > med.recovered());
  }
}

TEST_CASE("Testing decreasing susceptible people") {
  pandemic::Population med(10, 10);
  double beta = 0.7;
  double gamma = 0.4;
  SUBCASE("No vaccination") {
    int T = 10;
    int v_begin = 0;
    double v_eff = 0;
    pandemic::Population next = med;
    for (int i = 0; i != T; ++i) {
      next = evolve(next, beta, gamma, i, v_begin, v_eff);
    }
    CHECK(next.S_not_vax() < med.S_not_vax());
  }
  SUBCASE("Vaccination") {
    int T = 10;
    int v_begin = 2;
    double v_eff = 0.94;
    pandemic::Population next = med;
    for (int i = 0; i != T; ++i) {
      next = evolve(next, beta, gamma, i, v_begin, v_eff);
    }
    CHECK(next.S_not_vax() < med.S_not_vax());
  }
}
