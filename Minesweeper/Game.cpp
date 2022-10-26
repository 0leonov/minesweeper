#include "Game.h"

Game::Game(int width, int height, int bombCount)
{
	window = new RenderWindow(VideoMode(width * SCALE, height * SCALE), "Minesweeper", Style::Close);
	window->setFramerateLimit(30);

	Image image = Image{};
	image.loadFromFile("Resources/icon.png");
	window->setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());

	field = std::vector<std::vector<Cell*>>(width, std::vector<Cell*>(height));
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			field[j][i] = new Cell();
	
	this->bombCount = bombCount;
	this->width = width;
	this->height = height;

	SoundBuffer loseBuffer;
	loseBuffer.loadFromFile("Resources/lose.wav");
	loseSound.setBuffer(loseBuffer);

	SoundBuffer winBuffer;
	winBuffer.loadFromFile("Resources/win.wav");
	winSound.setBuffer(winBuffer);

	SoundBuffer clickBuffer;
	clickBuffer.loadFromFile("Resources/click.wav");
	clickSound.setBuffer(clickBuffer);

	Texture texture;
	texture.loadFromFile("Resources/tileset.jpg");
	sprite.setTexture(texture);

	start();
}

void Game::start()
{
	Clock clock;
    while (window->isOpen())
    {
		if (!isEnded)
			window->setTitle("Time: " + std::to_string(int(clock.getElapsedTime().asSeconds())) + " Bomb: " + std::to_string(bombCount - checkCount));

        update();
		
        display();
    }
}

void Game::update()
{
    Event event;
    while (window->pollEvent(event))
    {
        if (event.type == Event::Closed)
            window->close();

        if (event.type == Event::MouseButtonPressed)
        {
			int index_i = event.mouseButton.x / SCALE;
			int index_j = event.mouseButton.y / SCALE;

			if (event.mouseButton.button == Mouse::Right && !isEnded)
			{
				checkCell(index_i, index_j);
			}	
			else if (event.mouseButton.button == Mouse::Left && !isEnded)
			{
				if (firstCell == NULL)
				{
					firstCell = field[index_i][index_j];
					setBombs(bombCount);
				}

				openCell(index_i, index_j);
			}	
        }
    }
}

void Game::display()
{
    window->clear(Color(189, 189, 189));

	draw();

    window->display();
}

void Game::draw()
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			sprite.setPosition(float(i * SCALE), float(j * SCALE));

			if (field[i][j]->isChecked)
				sprite.setTextureRect(IntRect(11 * SCALE, 0, SCALE, SCALE));
			else if (!field[i][j]->isOpened)
				sprite.setTextureRect(IntRect(10 * SCALE, 0, SCALE, SCALE));
			else if (field[i][j]->isBomb)
				sprite.setTextureRect(IntRect(9 * SCALE, 0, SCALE, SCALE));
			else
				sprite.setTextureRect(IntRect(getBombAroud(i, j) * SCALE, 0, SCALE, SCALE));

			window->draw(sprite);
		}
	}
}

void Game::setBombs(int count)
{
	srand(time(NULL));
	for (int i = 0; i < count; i++)
	{
		Cell* cell;

		do { cell = field[rand() % width][rand() % height]; } 
		while (cell->isBomb || cell == firstCell);

		cell->isBomb = true;
	}
}

void Game::openCell(int i, int j)
{
	if (field[i][j]->isOpened || field[i][j]->isChecked) return;

	field[i][j]->isOpened = true;

	if (field[i][j]->isBomb == true)
	{
		lose();
		return;
	}

	if (getBombAroud(i, j) == 0)
		openAllAround(i, j);

	openedCellCount++;
	if (openedCellCount >= width * height - bombCount) win();

	clickSound.play();
}

void Game::checkCell(int i, int j)
{
	if (field[i][j]->isOpened == true) return;

	field[i][j]->isChecked = !field[i][j]->isChecked;

	if (field[i][j]->isChecked) checkCount++;
	else checkCount--;

	clickSound.play();
}

void Game::show()
{
	isEnded = true;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			field[j][i]->isChecked = false;
			field[j][i]->isOpened = true;
		}	
	}	
}

void Game::openAllAround(int i, int j)
{
	bool isUp = j == 0;
	bool isRight = i == width - 1;
	bool isDown = j == height - 1;
	bool isLeft = i == 0;

	if (!isUp) openCell(i, j - 1);
	if (!isRight) openCell(i + 1, j);
	if (!isDown) openCell(i, j + 1);
	if (!isLeft) openCell(i - 1, j);
	if (!isUp && !isRight) openCell(i + 1, j - 1);
	if (!isUp && !isLeft) openCell(i - 1, j - 1);
	if (!isDown && !isRight) openCell(i + 1, j + 1);
	if (!isDown && !isLeft) openCell(i - 1, j + 1);
}

void Game::countBombsAround(int i, int j)
{
	int count = 0;
	bool isUp = j == 0;
	bool isRight = i == width - 1;
	bool isDown = j == height - 1;
	bool isLeft = i == 0;

	if (!isUp && field[i][j - 1]->isBomb) count++;
	if (!isDown && field[i][j + 1]->isBomb) count++;
	if (!isRight && field[i + 1][j]->isBomb) count++;
	if (!isLeft && field[i - 1][j]->isBomb) count++;
	if (!isRight && !isUp && field[i + 1][j - 1]->isBomb) count++;
	if (!isRight && !isDown && field[i + 1][j + 1]->isBomb) count++;
	if (!isLeft && !isUp && field[i - 1][j - 1]->isBomb) count++;
	if (!isLeft && !isDown && field[i - 1][j + 1]->isBomb) count++;

	field[i][j]->bombAround = count;
}

int Game::getBombAroud(int i, int j)
{
	if (field[i][j]->bombAround == NULL)
		countBombsAround(i, j);

	return field[i][j]->bombAround;
}

void Game::win()
{
	winSound.play();

	std::cout << "You Win!\n";
	show();
}

void Game::lose()
{
	loseSound.play();

	std::cout << "You Lose\n";
	show();
}