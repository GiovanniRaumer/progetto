#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "evolve.hpp"


TEST_CASE("Testing immunity")
{
  pandemic::Population small(2,4);
  double beta=0.7;
  double gamma=0.4;
  SUBCASE("No vaccination"){
    int i=0;
    int v_begin=0;
    double v_eff=0;
    small.human(1,1).Is = pandemic::Human::R;
    CHECK(small.human(1,1).Is == pandemic::Human::R);

    auto const next = evolve(small, beta, gamma, i, v_begin, v_eff);
    CHECK(next.human(1,1).Is == small.human(1,1).Is);
  }
  SUBCASE("Vaccination"){
    int i=3;
    int v_begin=5;
    double v_eff=0.94;
    small.human(1,1).Is = pandemic::Human::R;
    CHECK(small.human(1,1).Is == pandemic::Human::R);

    auto const next = evolve(small, beta, gamma, i, v_begin, v_eff);
    CHECK(next.human(1,1).Is == small.human(1,1).Is);
  }
}
  
TEST_CASE("Testing increasing of vaccinated subjects"){
  pandemic::Population med(10,10);
  double beta=0.7;
  double gamma=0.4;
  int i=10;
  int v_begin=2;
  double v_eff=0.94;
  auto const next = evolve(med, beta, gamma, i, v_begin, v_eff);
  CHECK(next.vaccinated() > med.vaccinated());
}

TEST_CASE("Testing increasing of recovered subjects"){
  pandemic::Population med(10,10);
  double beta=0.7;
  double gamma=0.4;
  SUBCASE("No vaccination"){
    int T=10;
    int v_begin=0;
    double v_eff=0;
    pandemic::Population next;
    for (int i = 0; i != T; ++i) {
      next = evolve(med, beta, gamma, i, v_begin, v_eff);
      
    }
    CHECK(next.recovered() > med.recovered());
  }
  SUBCASE("Vaccination"){
    int i=10;
    int v_begin=2;
    double v_eff=0.94;
    auto const next = evolve(med, beta, gamma, i, v_begin, v_eff);
    CHECK(next.recovered() > med.recovered());
  }
}


