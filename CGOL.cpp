#include <SFML/Graphics.hpp>
#include <iostream>

typedef std::vector<std::vector<int>> matrix;
int width = 1000, height = 1000;
int cellSize = 2;
int cols, rows;
bool pause = false;
int pen;

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
			int neighborX = (i + k + rows) % rows;
			int neighborY = (j + l + cols) % cols;
			sum += grid[neighborX][neighborY];
		}
	}
	sum -= grid[i][j];
	return sum;
}

matrix randomStates()
{
	matrix grid;
	grid.resize(rows);
	for (int y = 0; y < rows; ++y) {
		grid[y].resize(cols);
		for (int x = 0; x < cols; ++x) {
			grid[y][x] = crand(2);
		}
	}
	return grid;
}

void draw(matrix &grid, int x, int y, int state)
{
	for (int i = -pen; i <= pen; ++i) {
		for (int j = -pen; j <= pen; ++j) {
			grid[(y + i + rows) % rows][(x + j + cols) % cols] = state;
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
	cols = width / cellSize;
	rows = height / cellSize;
	pen = rows / 50;
	matrix grid(rows, std::vector<int>(cols, 0));
	matrix nextGen = grid;
	sf::VertexArray cells(sf::Quads);
	sf::VertexArray lines(sf::Lines);

	if (cellSize > 2) {
		for (int x = 0; x < cols; ++x) {
			lines.append(sf::Vertex(sf::Vector2f(x * cellSize, 0), lineColor));
			lines.append(sf::Vertex(sf::Vector2f(x * cellSize, height - 1), lineColor));
		}
		for (int y = 0; y < rows; ++y) {
			lines.append(sf::Vertex(sf::Vector2f(0, y * cellSize), lineColor));
			lines.append(sf::Vertex(sf::Vector2f(width - 1, y * cellSize), lineColor));
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
					grid.assign(grid.size(), std::vector<int>(cols, 0));
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
				} else if (d >= rows / 3) {
					pen = rows / 3;
				} else { pen = d; }

			} else if (event.type == sf::Event::MouseButtonPressed) {
				window.setMouseCursorVisible(false);
			} else if (event.type == sf::Event::MouseButtonReleased) {
				window.setMouseCursorVisible(true);
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			int x = sf::Mouse::getPosition(window).x;
			int y = sf::Mouse::getPosition(window).y;

			if (x < 0) x = 0;
			if (y < 0) y = 0;
			if (x > width) x = width;
			if (y > height) y = height;
			sf::Mouse::setPosition(sf::Vector2i(x, y), window);

			draw(grid, x / cellSize, y / cellSize, 1);
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			int x = sf::Mouse::getPosition(window).x;
			int y = sf::Mouse::getPosition(window).y;

			if (x < 0) x = 0;
			if (y < 0) y = 0;
			if (x > width) x = width;
			if (y > height) y = height;
			sf::Mouse::setPosition(sf::Vector2i(x, y), window);

			draw(grid, x / cellSize, y / cellSize, 0);
		}

		cells.clear();
		window.clear(backgroudColor);
		for (int row = 0; row < rows; ++row) {
			for (int col = 0; col < cols; ++col) {
				float x = cellSize * col, y = cellSize * row;
				int state = grid[row][col];
				if (state == 1) {
					cells.append(sf::Vertex(sf::Vector2f(x, y), cellColor));
					cells.append(sf::Vertex(sf::Vector2f(x + cellSize, y), cellColor));
					cells.append(sf::Vertex(sf::Vector2f(x + cellSize, y + cellSize), cellColor));
					cells.append(sf::Vertex(sf::Vector2f(x, y + cellSize), cellColor));
				}
				int sum = countNeighbors(grid, row, col);
				nextGen[row][col] = state;
				if (!pause) {
					if (state == 1) {
						//Any live cell with more than three live neighbours dies, as if by overpopulation.
						if (sum > 3) { nextGen[row][col] = 0; }
							//Any live cell with two or three live neighbours lives on to the next generation.
						else if (sum >= 2) { nextGen[row][col] = 1; }
							//Any live cell with fewer than two live neighbours dies, as if by underpopulation.
						else { nextGen[row][col] = 0; }
						//Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
					} else {
						if (sum == 3) { nextGen[row][col] = 1; }
						else { nextGen[row][col] = state; }
					}
				}
			}
		}
		sf::RectangleShape brush(sf::Vector2f(2 * pen * cellSize, 2 * pen * cellSize));
		brush.setFillColor(sf::Color(cellColor.r, cellColor.g, cellColor.b, 64));
		brush.setPosition(
			sf::Mouse::getPosition(window).x - cellSize * pen,
			sf::Mouse::getPosition(window).y - cellSize * pen);

		window.draw(cells);
		window.draw(lines);
		window.draw(brush);
		window.display();
		grid = nextGen;
	}
	return 0;
}