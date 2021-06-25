#include "sir_v.hpp"

// COSTRUTTORE
pandemic::Population::Population(int N, int I0) : w_side(N), w_grid(N, Row(N)), first_I{I0} {
  assert(w_side > 0 && first_I > 0);

  for (int r = 0; r != N; ++r) {
    for (int c = 0; c != N; ++c) {
      w_grid[r][c].Is = Human::S;
    }
  }

  std::default_random_engine eng{std::random_device{}()};
  std::uniform_int_distribution<int> dist{0, w_side - 1};
  int r = dist(eng);
  int c = dist(eng);

  for (int i = 0; i != first_I; r = dist(eng), c = dist(eng)) {
    if (w_grid[r][c].Is == Human::S) {
      w_grid[r][c].Is = Human::I;
      ++i;
    }
  }
}

// ACCESSO A CELLE
pandemic::Human const& pandemic::Population::human(int r, int c) const noexcept {
  auto const i = (r + w_side) % w_side;
  auto const j = (c + w_side) % w_side;

  assert(i >= 0 && i < w_side && j >= 0 && i < w_side);
  return w_grid[i][j];
}

pandemic::Human& pandemic::Population::human(int r, int c) noexcept {
  auto const i = (r + w_side) % w_side;
  auto const j = (c + w_side) % w_side;

  assert(i >= 0 && i < w_side && j >= 0 && i < w_side);
  return w_grid[i][j];
}

// METODI CONTEGGIO
int pandemic::Population::infected() const {
  int infected = 0;
  for (int r = 0; r != w_side; ++r) {
    for (int c = 0; c != w_side; ++c) {
      if (w_grid[r][c].Is == Human::I) {
        ++infected;
      }
    }
  }
  return infected;
}

int pandemic::Population::recovered() const {
  int recovered = 0;
  for (int r = 0; r != w_side; ++r) {
    for (int c = 0; c != w_side; ++c) {
      if (w_grid[r][c].Is == Human::R) {
        ++recovered;
      }
    }
  }
  return recovered;
}

int pandemic::Population::vaccinated() const {
  int vaccinated = 0;
  for (int r = 0; r != w_side; ++r) {
    for (int c = 0; c != w_side; ++c) {
      if (w_grid[r][c].v == true) {
        ++vaccinated;
      }
    }
  }
  return vaccinated;
}

int pandemic::Population::S_not_vax() const {
  int notvax = 0;
  for (int r = 0; r != w_side; ++r) {
    for (int c = 0; c != w_side; ++c) {
      if (w_grid[r][c].v == false && w_grid[r][c].Is == Human::S) {
        ++notvax;
      }
    }
  }
  return notvax;
}
