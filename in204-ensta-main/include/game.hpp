#ifndef GAMEHPP
#define GAMEHPP

#include "piece.hpp"
#include "grid.hpp"
#include <SFML/Graphics.hpp>

void lineFullHandler(Grid& grid, bool test, int& score, int& nbTot);
bool isFull(std::vector<char> line);
int calculScore(int nbLinesFull);

void nextStep(Piece& piece, bool test);

#endif
