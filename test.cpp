#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "World.hpp"


TEST_CASE("Testing stable lives")
{
  life::World block(4);
  CHECK(block.cell(1,1) == life::Cell::Dead);
  block.cell(1,1) = life::Cell::Alive;
  CHECK(block.cell(1,1) == life::Cell::Alive);
  block.cell(1,2) = life::Cell::Alive;
  block.cell(2,1) = life::Cell::Alive;
  block.cell(2,2) = life::Cell::Alive;

  auto const next = life::evolve(block);
  CHECK(next == block);

}


TEST_CASE("Testing oscillators")
{
  life::World blinker(7);
  blinker.cell(1,2) = life::Cell::Alive;
  blinker.cell(2,2) = life::Cell::Alive;
  blinker.cell(3,2) = life::Cell::Alive;

  auto next = evolve(blinker);

  life::World expected(blinker.side());
  expected.cell(2,1) = life::Cell::Alive;
  expected.cell(2,2) = life::Cell::Alive;
  expected.cell(2,3) = life::Cell::Alive;

  CHECK(next == expected);

  next = evolve(next);

  CHECK(next == blinker);
}
