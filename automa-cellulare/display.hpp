#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdexcept>

#include "sirII.hpp"

namespace pandemic {

class Display {
  sf::RenderWindow& m_window;
  sf::Font m_font;

 public:
  Display(sf::RenderWindow& window)
      : m_window{window} 
        //m_font.loadFromFile("ZenDots-Regular.ttf");
  {
    // https://github.com/googlefonts/zen-dots
    if (!m_font.loadFromFile("ZenDots-Regular.ttf")) {
      if (!m_font.loadFromFile(
              "/usr/share/fonts/truetype/msttcorefonts/arialbi.ttf")) {
        throw std::runtime_error{"cannot load font"};
      }
    }
  }

  void draw(Population const& population) {
    int const N = population.side();
    sf::Vector2f const human_size{m_window.getSize() /static_cast<unsigned>(N)};
    sf::RectangleShape rect(human_size);
    // rect.setFillColor(sf::Color::Red);
    rect.setOutlineThickness(1.f);
    rect.setOutlineColor(sf::Color::White);

    for (int x = 0; x != N; ++x) {
      for (int y = 0; y != N; ++y) {
        if (population.human(x, y) == Human::Susceptible) {
          rect.setPosition(x * human_size.y, y * human_size.x);
          rect.setFillColor(sf::Color::Blue);
          m_window.draw(rect);
        } else if (population.human(x, y) == Human::Infected) {
          rect.setPosition(x * human_size.y, y * human_size.x);
          rect.setFillColor(sf::Color::Red);
          m_window.draw(rect);
        } else {
          rect.setPosition(x * human_size.y, y * human_size.x);
          rect.setFillColor(sf::Color::Blue);
          m_window.draw(rect);
        }
      }
    }
  }
  void show_message(std::string const& message) {
    sf::Text text{message, m_font};

    text.move((m_window.getSize().x - text.getLocalBounds().width) / 2,
              m_window.getSize().y / 4);
    text.setFillColor(sf::Color::Black);

    m_window.draw(text);
  }
};

}  // namespace pandemic

#endif