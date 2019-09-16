#include <SFML/Graphics.hpp>

typedef std::vector<std::vector<int>> matrix;
int size = 800;
float cellSize = 4;
int dimention = size / cellSize;
bool pause = false;

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

matrix genGrid()
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
	grid[y / cellSize][x / cellSize] = state;
}

int main()
{
	matrix grid = genGrid();
	matrix nextGen = grid;
	sf::VertexArray cells(sf::Quads);
	sf::VertexArray lines(sf::Lines);

	sf::Color cellColor = sf::Color::Black;
	sf::Color backgroudColor = sf::Color::White;
	sf::Color lineColor = sf::Color(128, 128, 128, 64);

	if (cellSize > 2) {
		for (int col = 0; col < dimention; ++col) {
			lines.append(sf::Vertex(sf::Vector2f(0, col * cellSize), lineColor));
			lines.append(sf::Vertex(sf::Vector2f(size - 1, col * cellSize), lineColor));
		}
		for (int row = 0; row < dimention; ++row) {
			lines.append(sf::Vertex(sf::Vector2f(row * cellSize, 0), lineColor));
			lines.append(sf::Vertex(sf::Vector2f(row * cellSize, size - 1), lineColor));
		}
	}

	sf::RenderWindow window(sf::VideoMode(size, size), "Conways", sf::Style::Close);
	window.setFramerateLimit(100);

	while (window.isOpen()) {
		sf::Event event = {};
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) window.close();
				if (event.key.code == sf::Keyboard::Space) pause = !pause;
				if (event.key.code == sf::Keyboard::Enter) grid = genGrid();
				if (event.key.code == sf::Keyboard::Delete) {
					grid.assign(grid.size(), std::vector<int>(dimention, 0));
				}
				if (event.key.code == sf::Keyboard::Num1) {
					cellColor = sf::Color::Black;
					backgroudColor = sf::Color::White;
				}if (event.key.code == sf::Keyboard::Num2) {
					cellColor = sf::Color::White;
					backgroudColor = sf::Color::Black;
				}
			}
		}
		cells.clear();
		window.clear(backgroudColor);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			int x = sf::Mouse::getPosition(window).x;
			int y = sf::Mouse::getPosition(window).y;

			if (x < size && y < size && x > 0 && y > 0) {
				draw(grid, x, y, 1);
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			int x = sf::Mouse::getPosition(window).x;
			int y = sf::Mouse::getPosition(window).y;

			if (x < size && y < size && x > 0 && y > 0) {
				draw(grid, x, y, 0);
			}
		}

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