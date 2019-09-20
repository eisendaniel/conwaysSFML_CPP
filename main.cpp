#include <SFML/Graphics.hpp>
#include <iostream>

typedef std::vector<std::vector<int>> matrix;
int size = 800;
int width = 800;
int height = 800;
float cellSize = 2;
int dimention = size / cellSize;
bool pause = false;
int pen = 0;

int crand(int depth)
{
	int r;
	do {
		r = rand();
	}
	while (r >= (RAND_MAX - RAND_MAX % depth));
	r %= depth;
	return r;
}

int countNeighbors(matrix &grid, int i, int j)
{
	int sum = 0;
	for (int k = -1; k < 2; ++k) {
		for (int l = -1; l < 2; ++l) {
			int neighborX = (i + k + dimention) % dimention;
			int neighborY = (j + l + dimention) % dimention;
			sum += grid[neighborX][neighborY];
		}
	}
	sum -= grid[i][j];
	return sum;
}

matrix randomStates()
{
	matrix grid;
	grid.resize(dimention);
	for (int i = 0; i < dimention; ++i) {
		grid[i].resize(dimention);
		for (int j = 0; j < dimention; ++j) {
			grid[i][j] = crand(2);
		}
	}
	return grid;
}

void draw(matrix &grid, int x, int y, int state)
{
	for (int i = -pen; i <= pen; ++i) {
		for (int j = -pen; j <= pen; ++j) {
			grid[(y + i + dimention) % dimention][(x + j + dimention) % dimention] = state;
		}
	}
}

int main()
{
	/*Setup window and framerate
	 * init default colors
	 * */
	sf::RenderWindow window(sf::VideoMode(width, height), "Conways", sf::Style::Close);
	window.setFramerateLimit(60);
	sf::Color cellColor = sf::Color::White;
	sf::Color backgroudColor = sf::Color::Black;
	sf::Color lineColor = sf::Color(128, 128, 128, 64);

	//restart and definition point
	restart:
	dimention = size / cellSize;
	pen = dimention / 50;
	matrix grid(dimention, std::vector<int>(dimention, 0));
	matrix nextGen = grid;
	sf::VertexArray cells(sf::Quads);
	sf::VertexArray lines(sf::Lines);

	if (cellSize > 2) {
		for (int x = 0; x < dimention; ++x) {
			lines.append(sf::Vertex(sf::Vector2f(0, x * cellSize), lineColor));
			lines.append(sf::Vertex(sf::Vector2f(size - 1, x * cellSize), lineColor));
		}
		for (int y = 0; y < dimention; ++y) {
			lines.append(sf::Vertex(sf::Vector2f(y * cellSize, 0), lineColor));
			lines.append(sf::Vertex(sf::Vector2f(y * cellSize, size - 1), lineColor));
		}
	}

	while (window.isOpen()) { //lifetime of program
		sf::Event event = {};
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) { window.close(); }
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) window.close();
				if (event.key.code == sf::Keyboard::Space) pause = !pause;
				if (event.key.code == sf::Keyboard::Enter) grid = randomStates();
				if (event.key.code == sf::Keyboard::Delete) {
					grid.assign(grid.size(), std::vector<int>(dimention, 0));
				} else if (event.key.code == sf::Keyboard::LBracket) {
					cellColor = sf::Color::Black;
					backgroudColor = sf::Color::White;
				} else if (event.key.code == sf::Keyboard::RBracket) {
					cellColor = sf::Color::White;
					backgroudColor = sf::Color::Black;
				} else if (event.key.code == sf::Keyboard::Num1) {
					if (cellSize != 1) {
						cellSize = 1;
						goto restart;
					}
				} else if (event.key.code == sf::Keyboard::Num2) {
					if (cellSize != 2) {
						cellSize = 2;
						goto restart;
					}
				} else if (event.key.code == sf::Keyboard::Num3) {
					if (cellSize != 5) {
						cellSize = 5;
						goto restart;
					}
				} else if (event.key.code == sf::Keyboard::Num4) {
					if (cellSize != 10) {
						cellSize = 10;
						goto restart;
					}
				}

			} else if (event.type == sf::Event::MouseWheelScrolled) {
				int d = pen + event.mouseWheelScroll.delta;
				if (d <= 0) {
					pen = 0;
				} else if (d >= dimention / 3) {
					pen = dimention / 3;
				} else { pen = d; }
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			int x = sf::Mouse::getPosition(window).x;
			int y = sf::Mouse::getPosition(window).y;

			draw(grid, x / cellSize, y / cellSize, 1);
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			int x = sf::Mouse::getPosition(window).x;
			int y = sf::Mouse::getPosition(window).y;

			draw(grid, x / cellSize, y / cellSize, 0);
		}

		cells.clear();
		window.clear(backgroudColor);
		for (int i = 0; i < dimention; ++i) {
			for (int j = 0; j < dimention; ++j) {
				float x = cellSize * j, y = cellSize * i;
				int state = grid[i][j];
				if (state == 1) {
					cells.append(sf::Vertex(sf::Vector2f(x, y), cellColor));
					cells.append(sf::Vertex(sf::Vector2f(x + cellSize, y), cellColor));
					cells.append(sf::Vertex(sf::Vector2f(x + cellSize, y + cellSize), cellColor));
					cells.append(sf::Vertex(sf::Vector2f(x, y + cellSize), cellColor));
				}
				int sum = countNeighbors(grid, i, j);
				nextGen[i][j] = state;
				if (!pause) {
					if (state == 1) {
						//Any live cell with more than three live neighbours dies, as if by overpopulation.
						if (sum > 3) {
							nextGen[i][j] = 0;
							//Any live cell with two or three live neighbours lives on to the next generation.
						} else if (sum >= 2) {
							nextGen[i][j] = 1;
							//Any live cell with fewer than two live neighbours dies, as if by underpopulation.
						} else { nextGen[i][j] = 0; }
						//Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
					} else {
						if (sum == 3) {
							nextGen[i][j] = 1;
						} else { nextGen[i][j] = state; }
					}
				}
			}
		}
		window.draw(lines);
		window.draw(cells);
		window.display();
		grid = nextGen;
	}
	return 0;
}