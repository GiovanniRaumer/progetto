#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "sirq.hpp"


TEST_CASE("Testing equations")
{
    State initial_state;
    initial_state.S=1000;
    initial_state.I=40;
    initial_state.b=0.7;
    initial_state.g=0.4;
    int T=20;

    Epidemic ep(initial_state);
    auto const evolution = ep.evolve(T);
    auto const last_state = evolution.back();

    SUBCASE("Testing conservation of total population")
    {
        CHECK(initial_state.S+initial_state.I+initial_state.R == last_state.S+last_state.I+last_state.R);
    }

    SUBCASE("Testing end of epidemic (Infected = 0 ?)")
    {
        CHECK(last_state.I == 0);
    }

    SUBCASE("")
    {}
}

