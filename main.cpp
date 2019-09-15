#include <SFML/Graphics.hpp>

int size = 1000;
float cellSize = 1;
int dimention = size / cellSize;
bool edit = true;

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

int countNeighbors(std::vector<std::vector<int>> &grid, int i, int j)
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

std::vector<std::vector<int>> genGrid()
{
	std::vector<std::vector<int>> grid;
	grid.resize(dimention);
	for (int i = 0; i < dimention; ++i) {
		grid[i].resize(dimention);
		for (int j = 0; j < dimention; ++j) {
			grid[i][j] = crand(2);
		}
	}
	return grid;
}

int main()
{
	sf::VertexArray cells(sf::Quads);
	std::vector<std::vector<int>> grid = genGrid();
	std::vector<std::vector<int>> nextGen = grid;

	sf::RenderWindow window(sf::VideoMode(size, size), "Conways");
	window.setFramerateLimit(100);


	while (window.isOpen()) {
		sf::Event event = {};
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) window.close();
				if (event.key.code == sf::Keyboard::Space) edit = !edit;
				if (event.key.code == sf::Keyboard::Delete) {
					grid.assign(grid.size(), std::vector<int>(dimention, 0));
				}
				if (event.key.code == sf::Keyboard::Enter) grid = genGrid();

			}
		}
		window.clear(sf::Color::Black);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			int x = sf::Mouse::getPosition(window).x;
			int y = sf::Mouse::getPosition(window).y;

			if (x < size && y < size && x > 0 && y > 0) {
				grid[y / cellSize][x / cellSize] = 1;
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			int x = sf::Mouse::getPosition(window).x;
			int y = sf::Mouse::getPosition(window).y;

			if (x < size && y < size && x > 0 && y > 0) {
				grid[y / cellSize][x / cellSize] = 0;
			}
		}

		for (int i = 0; i < dimention; ++i) {
			for (int j = 0; j < dimention; ++j) {
				float x = cellSize * j, y = cellSize * i;
				int state = grid[i][j];
				if (state == 1) {
					cells.append(sf::Vertex(sf::Vector2f(x, y), sf::Color::White));
					cells.append(sf::Vertex(sf::Vector2f(x+cellSize, y), sf::Color::White));
					cells.append(sf::Vertex(sf::Vector2f(x+cellSize, y+cellSize), sf::Color::White));
					cells.append(sf::Vertex(sf::Vector2f(x, y+cellSize), sf::Color::White));
				}

				int sum = countNeighbors(grid, i, j);
				nextGen[i][j] = state;
				if (!edit) {
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
						if (sum == 3) {
							nextGen[i][j] = 1;
						} else { nextGen[i][j] = state; }
					}
				}
			}
		}
		window.draw(cells);
		cells.clear();
		window.display();
		grid = nextGen;
	}
	return 0;
}