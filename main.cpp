#include <iostream>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <unistd.h>

int randomN(int depth)
{
	int r;
	do {
		r = rand();
	}
	while (r >= (RAND_MAX - RAND_MAX % depth));
	r %= depth;
	return r;
}

int main()
{

	int size = 500;
	float cellSize = 5;
	const int dimention = size / cellSize;

	std::vector<std::vector<int>> grid;
	for (int i = 0; i < dimention; ++i) {
		grid.resize(dimention);
		for (int j = 0; j < dimention; ++j) {
			grid[i].resize(dimention);
			grid[i][j] = (randomN(2));
		}
	}
	std::vector<std::vector<int>> nextGen = grid;

	sf::RenderWindow window(sf::VideoMode(size, size), "Conways");
	while (window.isOpen()) {
		sf::Event event = {};
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) window.close();
			}
		}

		for (int i = 0; i < dimention; ++i) {
			for (int j = 0; j < dimention; ++j) {
				float x = cellSize * j, y = cellSize * i;
				int state = grid[i][j];
				sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
				cell.setPosition(x, y);
				switch (state) {
					case 0:
						cell.setFillColor(sf::Color::White);
						cell.setOutlineColor(sf::Color(0, 0, 0, 64));
						cell.setOutlineThickness(1);
						break;
					case 1:
						cell.setFillColor(sf::Color::Black);
						cell.setOutlineColor(sf::Color(0, 0, 0, 64));
						cell.setOutlineThickness(1);
						break;
					default:
						break;
				}

				//count neighbors
				if (i == 0 || i == dimention - 1 || j == 0 || j == dimention - 1) {
					nextGen[i][j] = 0;
				} else {
					int sum = 0;
					for (int k = -1; k < 2; ++k) {
						for (int l = -1; l < 2; ++l) {
							sum += grid[i + k][j + l];
						}
					}
					sum -= state;

					if (state == 1) {
						//Any live cell with more than three live neighbours dies, as if by overpopulation.
						if (sum > 3) {
							nextGen[i][j] = 0;
							//Any live cell with two or three live neighbours lives on to the next generation.
						} else if (sum >= 2) {
							nextGen[i][j] = 1;
							//Any live cell with fewer than two live neighbours dies, as if by underpopulation.
						} else {
							nextGen[i][j] = 0;
						}
						//Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
					} else {
						if (sum == 3){
							nextGen[i][j] = 1;
						} else	nextGen[i][j] = state;
					}
				}
				window.draw(cell);
			}
		}
		window.display();
		grid = nextGen;
	}
	return 0;
}