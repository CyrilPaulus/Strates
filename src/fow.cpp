#include "fow.h"

FoW::FoW(Map *map, TileMap *tileMap, FogType type) :
  Map(*map, tileMap)
{
  this->map = map;
  origin = type;

  status = new FogStatus*[height];
  for (int i = 0; i < height; i++) {
    status[i] = new FogStatus[width];
  }

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      status[i][j] = HIDDEN;
      tiles[i][j] = ALL + origin;
    }
  }
}

FoW::~FoW() {
  for (int i = 0; i < height; i++)
    delete[] status[i];
  delete[] status;
}

void FoW::set(int x, int y, FogStatus value) {
  if (status[y][x] == value)
    return;

  status[y][x] = value;

  if (value == REVEALED)
    tiles[y][x] = NONE + origin;
  else
    updateTile(x, y);

  updateNeighbours(x, y);
}

void FoW::set(const sf::Vector2i &coords, FogStatus value) {
  set(coords.x, coords.y, value);
}

struct neighbour_t {
  int i, j;
  FoW::TileIndex rmIndex;
};

static const size_t neighbours_len = 8;
static const neighbour_t neighbours[] = {
  {  1,  0, FoW::S },
  {  1,  1, FoW::SE },
  {  0,  1, FoW::E },
  { -1,  1, FoW::NE },
  { -1,  0, FoW::N },
  { -1, -1, FoW::NW },
  {  0, -1, FoW::W },
  {  1, -1, FoW::SW },
};

static const int test[256] = {
  12,  0,  5,  0,  1,  8,  1,  8,
   6,  8,  1,  8,  1,  8,  1,  8,
   2, 13,  9, 13,  9, 13,  9, 13,
   2, 13,  9, 13,  9, 13,  9, 13,
   7, 11, 15, 11,  9, 13,  9, 13,
   2, 13,  9, 13,  9, 13,  9, 13,
   2, 13,  9, 13,  9, 13,  9, 13,
   2, 13,  9, 13,  9, 13,  9, 13,
   3, 11, 11, 11, 13, 13, 13, 13,
  10, 13, 13, 13, 13, 13, 13, 13,
  10, 13, 13, 13, 13, 13, 13, 13,
  10, 13, 13, 13, 13, 13, 13, 13,
   3, 11, 11, 11, 13, 13, 13, 13,
  10, 13, 13, 13, 13, 13, 13, 13,
  10, 13, 13, 13, 13, 13, 13, 13,
  10, 13, 13, 13, 13, 13, 13, 13,

   4,  0,  0,  0,  8,  8,  8,  8,
  14,  8,  8,  8,  8,  8,  8,  8,
  10, 13, 13, 13, 13, 13, 13, 13,
  10, 13, 13, 13, 13, 13, 13, 13,
   3, 11, 11, 11, 13, 13, 13, 13,
  10, 13, 13, 13, 13, 13, 13, 13,
  10, 13, 13, 13, 13, 13, 13, 13,
  10, 13, 13, 13, 13, 13, 13, 13,
   3, 11, 11, 11, 13, 13, 13, 13,
  10, 13, 13, 13, 13, 13, 13, 13,
  10, 13, 13, 13, 13, 13, 13, 13,
  10, 13, 13, 13, 13, 13, 13, 13,
   3, 11, 11, 11, 13, 13, 13, 13,
  10, 13, 13, 13, 13, 13, 13, 13,
  10, 13, 13, 13, 13, 13, 13, 13,
  10, 13, 13, 13, 13, 13, 13, 13,

};

void FoW::updateNeighbours(int x, int y) {
  for (size_t i = 0; i < neighbours_len; i++)
    updateTile(x + neighbours[i].j, y + neighbours[i].i);
}

void FoW::updateTile(int x, int y) {
  const neighbour_t *n = neighbours; // for shortness
  if (!contains(x, y) || status[y][x] == REVEALED)
    return;

  int index = 0;
  for (ssize_t i = neighbours_len - 1; i >= 0; i--) {
    int bit;
    if (contains(x + n[i].j, y + n[i].i))
      bit = (status[y + n[i].i][x + n[i].j] == REVEALED);
    else
      bit = 0;
    index = index * 2 + bit;
  }

  tiles[y][x] = test[index] + origin;
}
