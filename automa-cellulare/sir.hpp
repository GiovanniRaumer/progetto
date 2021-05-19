#ifndef SIR_HPP
#define SIR_HPP

#include "simpsir.hpp"
#include <random>

namespace pandemic
{
    struct Human
    {
        enum IS : char //unscoped enum; potrebbe dare problemi, ma dovrebbe andare bene...
        {
            S,
            I,
            R
        };

        IS Is;
        int d = 0; //stands for 'days'
    };

    class Population
    {
        using Row = std::vector<Human>;
        using Grid = std::vector<Row>;
        int w_side;
        Grid w_grid;
        int first_I;
        /*
        friend bool operator==(Population const &left, Population const &right) // operator== non è una member-function --> friend per accedere ai dati privati della classe
        {
            return left.w_grid == right.w_grid;
        }
        friend bool operator!=(Population const &left, Population const &right)
        {
            return left.w_grid != right.w_grid;
        }
*/
    public:
        Population(int N, int I0) : w_side(N), w_grid(N, Row(N)), first_I{I0}
        {
            assert(w_side > 0 && first_I > 0);
            for (int r = 0; r != N; ++r)
            {
                for (int c = 0; c != N; ++c)
                {
                    w_grid[r][c].Is = Human::S;
                }
            }

            std::default_random_engine eng{std::random_device{}()};
            std::uniform_int_distribution<int> dist{0, w_side - 1};

            for (int i = 0; i != first_I; ++i)
            {
                int r = dist(eng);
                int c = dist(eng);
                w_grid[r][c].Is = Human::I;
            }
        }

        int side() const { return w_side; }

        Human const &human(int r, int c) const noexcept
        {
            auto const i = (r + w_side) % w_side;
            auto const j = (c + w_side) % w_side;

            assert(i >= 0 && i < w_side && j >= 0 && i < w_side);
            return w_grid[i][j];
        }

        Human &human(int r, int c) noexcept
        {
            auto const i = (r + w_side) % w_side;
            auto const j = (c + w_side) % w_side;

            assert(i >= 0 && i < w_side && j >= 0 && i < w_side);
            return w_grid[i][j];
        }

        int infected() const
        {
            int infected = 0;
            for (int r = 0; r != w_side; ++r)
            {
                for (int c = 0; c != w_side; ++c)
                {
                    if (w_grid[r][c].Is == Human::I)
                    {
                        ++infected;
                    }
                }
            }
            return infected;
        }

        int recovered() const
        {
            int recovered = 0;
            for (int r = 0; r != w_side; ++r)
            {
                for (int c = 0; c != w_side; ++c)
                {
                    if (w_grid[r][c].Is == Human::R)
                    {
                        ++recovered;
                    }
                }
            }
            return recovered;
        }
    };

    inline int contacts(Population const &population, int r, int c)
    {
        int counter = 0;
        for (int i : {-1, 0, 1})
        {
            for (int j : {-1, 0, 1})
            {
                if (r + i == -1 || r + i == population.side() || c + j == -1 || c + j == population.side())
                {
                    continue;
                }
                else if (population.human(r + i, c + j).Is == Human::S)
                {
                    ++counter;
                }
            }
        }
        return counter;
    }

    inline Population evolve(Population const &current, situation::State state)
    {
        int const N = current.side();
        Population next = current; //costruttore di copia? implicito

        int const R0 = state.b / state.g;
        int const time = 1 / state.g;

        for (int r = 0; r != N; ++r)
        {
            for (int c = 0; c != N; ++c)
            {
                if (current.human(r, c).Is == Human::I)
                {
                    std::default_random_engine eng{std::random_device{}()};
                    std::uniform_int_distribution<int> dist{-1, 1};

                    for (int i = 0; i != R0; ++i)
                    {
                        auto j = dist(eng);
                        auto k = dist(eng);
                        for (; current.human(r + j, c + k).Is == Human::I || current.human(r + j, c + k).Is == Human::R; j = dist(eng), k = dist(eng))
                            ;
                        next.human(r + j, c + k).Is = Human::I;
                    }
                }
                if (current.human(r, c).Is == Human::I)
                {
                    ++next.human(r, c).d;
                }
                if (current.human(r, c).Is == Human::I && next.human(r, c).d == time)
                {
                    next.human(r, c).Is = Human::R;
                }
            }
        }
        return next;
    }
} // namespace pandemic

#endif

//TRUMP
//PULIRE FILE DA VARIABILI INUTILI