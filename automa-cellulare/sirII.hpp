#ifndef SIRII_HPP
#define SIRII_HPP

#include <iostream>
#include <vector>

namespace pandemic {

enum class Human: char {Susceptible,Infected,Recovered};

class Population {
  using Row = std::vector<Human>;
  using Grid = std::vector<Row>;
  int m_side;
  Grid m_grid;

  friend bool operator==(Population const& left, Population const& right) {  // ripassa il motivo di friend per l'esame
    return left.m_grid == right.m_grid;
  }
  friend bool operator!=(Population const& left, Population const& right) {
    return left.m_grid != right.m_grid;
  }

 public:
  Population(int N)
      : m_side(N),m_grid(N, Row(N, Human::Susceptible)) {  //vector con tonde:inizializza un vettore lungo N con gli elementi dopo la virgola
  }  
  Human const& human(int r, int c) const { 
    return m_grid[r][c]; 
    }
  Human& human(int r, int c) { 
    return m_grid[r][c]; 
    }
  int side() const { return m_side; }
};

inline int contacts(Population const& population, int x, int y) {
  int counter = 0;
  //if 
  for (int i : {-1, 0, 1}) {
    for (int j : {-1, 0, 1}) {
      if (i == 0 && j == 0) {
        continue;
      }
      if (x+i<=0 || (x+i>=population.side()) || y+j<=0 || y+j>=population.side()) { //sistemare-->unire i bordi
        continue;
      }
      if (population.human(x + i, y + j) == Human::Infected) {  //I oppure S?
        ++counter;
      }
    }
  }
  return counter;
}


inline Population evolve(Population const& current) {
  int const N = current.side();
  Population next(N);
  for (int r = 0; r != N; ++r) {
    for (int c = 0; c != N; ++c) {
      int const m=contacts(current, r, c);
      if (current.human(r,c)==Human::Susceptible && m==3) {
        next.human(r, c) = Human::Infected;
      } 
      else if (current.human(r,c)==Human::Infected && m==2) {
        next.human(r, c) = Human::Infected;
      }
  }
}
return next;
}

bool operator==(Population const& left, Population const& right);
bool operator!=(Population const& left, Population const& right);


}  // namespace pandemic

#endif
