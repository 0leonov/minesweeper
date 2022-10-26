#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

class Cell
{
public:
	bool isBomb = false;
	bool isOpened = false;
	bool isChecked = false;
	int bombAround;
};