#include "sir.hpp"
#include <chrono>
#include <thread>
#include <string> //per std::stod

void print(pandemic::Population const &population, situation::State state)
{
    const auto N = population.side();

    std::cout << "\033[2J";
    std::cout << "| S : " << state.S << " | " << "I : " << state.I << " | " << "R : " << state.R << " | " <<'\n';
    std::cout << '+' << std::string(N, '-') << "+\n";
    for (int r = 0; r != N; ++r)
    {
        std::cout << '|';
        for (int c = 0; c != N; ++c)
        {
            switch (population.human(r, c).Is)
            {
            case pandemic::Human::I:
                std::cout << "\033[31mX\033[0m";
                break;
            case pandemic::Human::R:
                std::cout << "\033[33mO\033[0m";
                break;
            default:
                std::cout << "\033[36mO\033[0m";
                break;
            }
        }
        std::cout << '|' << '\n';
    }
    std::cout << '+' << std::string(N, '-') << "+\n";
}

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 6)
        {
            throw std::runtime_error{"5 inputs required"};
        }

        for (int i = 1; i != 4; ++i)
        {
            if (atoi(argv[i]) <= 0)
            {
                throw std::runtime_error{
                    "inputs must be positive and different from zero"};
            }
        }

        for (int i = 4; i != 6; ++i)
        {
            if (std::stod(argv[i]) <= 0 || std::stod(argv[i]) > 1)
            {
                throw std::runtime_error{
                    "gamma and beta must be positive and less than 1"};
            }
        }

        int const N = std::atoi(argv[1]);
        int const I0 = std::atoi(argv[2]);
        int const T = std::atoi(argv[3]);
        double beta = std::stod(argv[4]);
        double gamma = std::stod(argv[5]);

        pandemic::Population population(N, I0);
        situation::State state;
        state.S = N * N - I0;
        state.I = I0;
        state.R = 0;
        state.b = beta;
        state.g = gamma;

        for (int i = 0; i != T; ++i)
        {
            print(population, state);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if (state.I == 0)
            {
                break;
            }
            state = situation::evolve(state);
            population = evolve(population, state);
        }
    }
    catch (std::runtime_error const &e)
    {
        std::cerr << e.what() << '\n';
    }
}