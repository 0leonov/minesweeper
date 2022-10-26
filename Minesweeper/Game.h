#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include "Cell.h"

using namespace sf;

class Game
{
public:
	Game(int width, int height, int bombCount);
	
	void start();
private:
	RenderWindow* window;

	const int SCALE = 32;
	int width;
	int height;

	std::vector<std::vector<Cell*>> field;

	int bombCount = 0;
	int openedCellCount = 0;
	int checkCount = 0;

	bool isEnded = false;

	Cell* firstCell;

	Sound loseSound;
	Sound winSound;
	Sound clickSound;

	Sprite sprite;

	void update();
	void display();
	
	void draw();

	void setBombs(int count);
	void openCell(int i, int j);
	void checkCell(int i, int j);
	void show();

	void openAllAround(int i, int j);
	void countBombsAround(int i, int j);
	int getBombAroud(int i, int j);

	void win();
	void lose();
};