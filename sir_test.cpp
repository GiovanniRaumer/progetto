#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "sir.hpp"


TEST_CASE("Testing conservation of total population")
{
    State initial_state;
    initial_state.S=1000;
    initial_state.I=40;
    initial_state.b=0.7;
    initial_state.g=0.4;
    int T=20;
    Epidemic ep(initial_state);
        
    auto const next = initial_state.evolve(T);
    CHECK(initial_state.S+initial_state.I+initial_state.R == next.S+next.I+next.R);


}

