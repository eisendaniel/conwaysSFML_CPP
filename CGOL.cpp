#include <SFML/Graphics.hpp>
#include <iostream>
#include "Cell.hpp"

typedef std::vector<std::vector<Cell>> matrix;
int width = 500, height = 500;
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

int countNeighbors(matrix &grid, int row, int col)
{
	int sum = 0;
	for (int i = -1; i < 2; ++i) {
		for (int j = -1; j < 2; ++j) {
			int neighborX = (row + i + rows) % rows;
			int neighborY = (col + j + cols) % cols;
			sum += grid[neighborX][neighborY].isAlive();
		}
	}
	sum -= grid[row][col].isAlive();
	return sum;
}

matrix randomStates()
{
	matrix grid;
	grid.resize(rows);
	for (int y = 0; y < rows; ++y) {
		grid[y].resize(cols);
		for (int x = 0; x < cols; ++x) {
			grid[y][x].setAge(crand(2));
		}
	}
	return grid;
}

void draw(matrix &grid, int x, int y, int state)
{
	for (int i = -pen; i <= pen; ++i) {
		for (int j = -pen; j <= pen; ++j) {
			grid[(y + i + rows) % rows][(x + j + cols) % cols].setAge(state);
		}
	}
}

int main()
{
	/*Setup window and frame-rate
	 * init default colors
	 * */
	sf::RenderWindow window(sf::VideoMode(width, height), "Conways", sf::Style::Close);
	window.setFramerateLimit(60);
	sf::Color cellColor[] = {sf::Color::White, sf::Color::Cyan, sf::Color::Green, sf::Color::Red};
	sf::Color brushColor(255, 255, 255, 64);
	sf::Color backgroudColor = sf::Color::Black;
	sf::Color lineColor = sf::Color(128, 128, 128, 64);

	//restart and definition point
	restart:
	cols = width / cellSize;
	rows = height / cellSize;
	pen = rows / 50;
	matrix grid(rows, std::vector<Cell>(cols, Cell()));
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
					grid.assign(grid.size(), std::vector<Cell>(cols, Cell()));
				} else if (event.key.code == sf::Keyboard::LBracket) {
					cellColor[0] = sf::Color::Black;
					backgroudColor = sf::Color::White;
				} else if (event.key.code == sf::Keyboard::RBracket) {
					cellColor[0] = sf::Color::White;
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

		cells.clear();
		window.clear(backgroudColor);

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

		for (int row = 0; row < rows; ++row) {
			for (int col = 0; col < cols; ++col) {
				float x = cellSize * col, y = cellSize * row;
				Cell cell = grid[row][col];
				if (cell.isAlive()) {
					cells.append(sf::Vertex(sf::Vector2f(x, y), cellColor[cell.getAge()]));
					cells.append(sf::Vertex(sf::Vector2f(
						x + cellSize, y), cellColor[cell.getAge()]));
					cells.append(sf::Vertex(sf::Vector2f(
						x + cellSize, y + cellSize), cellColor[cell.getAge()]));
					cells.append(sf::Vertex(sf::Vector2f(x, y
										+ cellSize), cellColor[cell.getAge()]));
				}
				int sum = countNeighbors(grid, row, col);
				nextGen[row][col] = cell;
				if (!pause) {
					if (cell.isAlive()) {
						//Any live Cell with more than three live neighbours dies, as if by overpopulation.
						if (sum > 3) { nextGen[row][col].kill(); }
							//Any live Cell with two or three live neighbours lives on to the next generation.
						else if (sum >= 2) { nextGen[row][col].grow(); }
							//Any live Cell with fewer than two live neighbours dies, as if by underpopulation.
						else { nextGen[row][col].kill(); }
						//Any dead Cell with exactly three live neighbours becomes a live Cell, as if by reproduction.
					} else {
						if (sum == 3) { nextGen[row][col].birth(); }
					}
				}
			}
		}
		sf::RectangleShape brush(sf::Vector2f(2 * pen * cellSize, 2 * pen * cellSize));
		brush.setFillColor(brushColor);
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