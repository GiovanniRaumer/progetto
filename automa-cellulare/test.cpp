#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "sir.hpp"


TEST_CASE("Testing immunity")
{
  pandemic::Population small(2,4);
  situation::State state;
  state.b=0.7;
  state.g=0.4;
  small.human(1,1).Is = pandemic::Human::R;
  CHECK(small.human(1,1).Is == pandemic::Human::R);

  auto const next = pandemic::evolve(small,state);
  CHECK(next.human(1,1).Is == small.human(1,1).Is);
}

TEST_CASE("Testing conservation of total population")
{
  pandemic::Population med(8,8);
  situation::State state;
  state.b=0.7;
  state.g=0.4;

  auto const next = pandemic::evolve(med,state);
  CHECK(next.infected()+next.S_not_vax()+next.recovered()+next.vaccinated() == med.side()*med.side());
}
