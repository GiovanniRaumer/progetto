#include <chrono>
#include <iostream>
#include <random>
#include <thread>

#include "display.hpp"
#include "sirII.hpp"

int main(int argc, char* argv[]) {
  try {
    if (argc != 6) {
      throw std::runtime_error{"5 inputs required"};
    }

    for (int i = 1; i != 4; ++i) {
      if (atoi(argv[i]) <= 0) {
        throw std::runtime_error{
            "inputs must be positive and different from zero"};
      }
    }

    if (std::stod(argv[4]) > 1 || std::stod(argv[5]) > 1 ||
        std::stod(argv[4]) <= 0. || std::stod(argv[5]) <= 0.) {
      throw std::runtime_error{
          "gamma and beta must be positive and less than 1"};
    }

    if (std::stod(argv[2]) > (std::stod(argv[1])*std::stod(argv[1]))) {
      throw std::runtime_error{"Infected must be equal less than Susceptible"};
    }

    int l = std::atoi(argv[1]);
    int I0 = std::atoi(argv[2]);
    //int T = std::atoi(argv[3]);
    //double beta = std::stod(argv[4]);
    //double gamma = std::stod(argv[5]);

    int grid_size = l;
    constexpr int human_size = 5;

    sf::RenderWindow window(
        sf::VideoMode(grid_size * human_size, grid_size * human_size),
        "SIR simulation",
        sf::Style::Close);  // rivedi lo stile
    window.setVerticalSyncEnabled(true);

    pandemic::Display display{window};
    pandemic::Population population{grid_size};

    std::default_random_engine eng{std::random_device{}()};
    std::uniform_int_distribution<int> dist{0, grid_size - 1};

    for (int i = 0; i != I0; ++i) {
      auto r = dist(eng);
      auto c = dist(eng);
      for (; population.human(r, c) == pandemic::Human::Infected;
           r = dist(eng), c = dist(eng))
        ;
      population.human(r, c) = pandemic::Human::Infected;
    }

    if (window.isOpen()) {
      window.clear(sf::Color::White);
      display.draw(population);
      display.show_message("Press any key to start/stop");
      window.display();

      sf::Event event;
      while (window.waitEvent(event)) {
        if (event.type == sf::Event::KeyPressed) {
          break;
        } else if (event.type == sf::Event::Closed) {
          window.close();
        }
      }
    }

    bool running = true;
    while (window.isOpen()) {
      // process events
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::KeyPressed) {
          running = !running;
        } else if (event.type == sf::Event::Closed) {
          window.close();
        }
      }

      window.clear(sf::Color::White);

      if (running) {
        population = evolve(population);
      }
      display.draw(population);

      window.display();
    }
  }
catch (std::runtime_error const& e) {
  std::cerr << e.what() << '\n';
}

}

