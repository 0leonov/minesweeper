#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"

int main()
{
	while (true)
	{
		int difficulty;
		do
		{
			std::cout << "Choose the difficulty:\n - Beginner - 0\n - Intermediate - 1\n - Expert - 2\n";
			std::cin >> difficulty;
		} while (difficulty < 0 || difficulty > 2);
		system("cls");

		if (difficulty == 0)
			Game* game = new Game(9, 9, 10);
		else if (difficulty == 1)
			Game* game = new Game(16, 16, 40);
		else
			Game* game = new Game(30, 16, 99);
	}	
}