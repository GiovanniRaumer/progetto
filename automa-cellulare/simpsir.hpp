#ifndef SIMP_SIR_HPP
#define SIMP_SIR_HPP

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

namespace situation
{

    struct State
    { 
        int S;
        int I;
        int R;
        int I_last = 0;
        int R_last = 0;
        double b;
        double g;
    };

    State evolve(State const &state)
    {
        //uso due stati, oppure sovrascrivo un solo stato?
        //con un solo stato, però, devo usare un vettore e un algoritmo per controllare quando stati sono tutti uguali (algo che trova i primi due uguali adiacenti in un vettore...)
        int const N = state.S + state.I + state.R;
        State next;
        next.S = round(state.S - state.b * state.S * state.I / N);
        next.I = round(state.I + state.b * state.S * state.I / N - state.g * state.I);
        next.R = N - next.S - next.I;
        next.b = state.b;
        next.g = state.g;
        
        if (next.S == state.S && next.I == state.I && next.R == state.R)
        {
            next.I = next.I - 1;
            next.R = next.R + 1;
        }
        next.R_last = next.R - state.R;
        next.I_last = next.I - state.I + next.R_last;
        assert(N == next.S + next.I + next.R);
        return next;
    }
}

#endif