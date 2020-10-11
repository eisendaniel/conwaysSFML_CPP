#include "Cell.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

typedef std::vector<std::vector<Cell>> matrix;
int width = 1000, height = 1000;
int cell_size = 2;
int cols, rows;
bool pause = false;
int pen;

int crand(int depth)
{
	int r;
	do
	{
		r = rand();
	} while (r >= (RAND_MAX - RAND_MAX % depth));
	r %= depth;
	return r;
}

int count_neighbors(matrix &grid, int row, int col)
{
	int sum = 0;
	for (int i = -1; i < 2; ++i)
	{
		for (int j = -1; j < 2; ++j)
		{
			int neighborX = (row + i + rows) % rows;
			int neighborY = (col + j + cols) % cols;
			sum += grid[neighborX][neighborY].is_alive();
		}
	}
	sum -= grid[row][col].is_alive();
	return sum;
}

matrix random_states()
{
	matrix grid;
	grid.resize(rows);
	for (int y = 0; y < rows; ++y)
	{
		grid[y].resize(cols);
		for (int x = 0; x < cols; ++x)
		{
			grid[y][x].set_age(crand(2));
		}
	}
	return grid;
}

void draw(matrix &grid, int x, int y, int state)
{
	for (int i = -pen; i <= pen; ++i)
	{
		for (int j = -pen; j <= pen; ++j)
		{
			grid[(y + i + rows) % rows][(x + j + cols) % cols].set_age(state);
		}
	}
}

int main()
{
	/*Setup window and frame-rate
   * init default colors
   * */
	sf::RenderWindow window(sf::VideoMode(width, height), "Conways",
							sf::Style::Close);
	window.setFramerateLimit(60);

	sf::Color one(0, 255, 255, 255);
	sf::Color two(0, 255, 0, 255);
	sf::Color three(255, 0, 0, 255);

	sf::Color cellColor[] = {sf::Color::White, one, two, three};
	sf::Color brushColor(255, 255, 255, 64);
	sf::Color backgroundColor(32, 32, 32);
	sf::Color lineColor(128, 128, 128, 64);

// restart and definition point
restart:
	cols = width / cell_size;
	rows = height / cell_size;
	pen = rows / 50;
	matrix grid(rows, std::vector<Cell>(cols, Cell()));
	matrix nextGen = grid;
	sf::VertexArray cells(sf::Quads);
	sf::VertexArray lines(sf::Lines);

	if (cell_size > 2)
	{
		for (int x = 0; x < cols; ++x)
		{
			lines.append(sf::Vertex(sf::Vector2f(x * cell_size, 0), lineColor));
			lines.append(
				sf::Vertex(sf::Vector2f(x * cell_size, height - 1), lineColor));
		}
		for (int y = 0; y < rows; ++y)
		{
			lines.append(sf::Vertex(sf::Vector2f(0, y * cell_size), lineColor));
			lines.append(
				sf::Vertex(sf::Vector2f(width - 1, y * cell_size), lineColor));
		}
	}

	while (window.isOpen())
	{ // lifetime of program
		sf::Event event = {};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
				if (event.key.code == sf::Keyboard::Space)
					pause = !pause;
				if (event.key.code == sf::Keyboard::Enter)
					grid = random_states();
				if (event.key.code == sf::Keyboard::Delete)
				{
					grid.assign(grid.size(), std::vector<Cell>(cols, Cell()));
				}
				else if (event.key.code == sf::Keyboard::LBracket)
				{
					cellColor[0] = sf::Color::Black;
					backgroundColor = sf::Color::White;
				}
				else if (event.key.code == sf::Keyboard::RBracket)
				{
					cellColor[0] = sf::Color::White;
					backgroundColor = sf::Color::Black;
				}
				else if (event.key.code == sf::Keyboard::Num1)
				{
					if (cell_size != 1)
					{
						cell_size = 1;
						goto restart;
					}
				}
				else if (event.key.code == sf::Keyboard::Num2)
				{
					if (cell_size != 2)
					{
						cell_size = 2;
						goto restart;
					}
				}
				else if (event.key.code == sf::Keyboard::Num3)
				{
					if (cell_size != 5)
					{
						cell_size = 5;
						goto restart;
					}
				}
				else if (event.key.code == sf::Keyboard::Num4)
				{
					if (cell_size != 10)
					{
						cell_size = 10;
						goto restart;
					}
				}
				else if (event.key.code == sf::Keyboard::Num5)
				{
					if (cell_size != 20)
					{
						cell_size = 20;
						goto restart;
					}
				}
			}
			else if (event.type == sf::Event::MouseWheelScrolled)
			{
				int d = pen + event.mouseWheelScroll.delta;
				if (d <= 0)
				{
					pen = 0;
				}
				else if (d >= rows / 3)
				{
					pen = rows / 3;
				}
				else
				{
					pen = d;
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				window.setMouseCursorVisible(false);
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				window.setMouseCursorVisible(true);
			}
		}

		cells.clear();
		window.clear(backgroundColor);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			int x = sf::Mouse::getPosition(window).x;
			int y = sf::Mouse::getPosition(window).y;

			if (x < 0)
				x = 0;
			if (y < 0)
				y = 0;
			if (x > width)
				x = width;
			if (y > height)
				y = height;
			sf::Mouse::setPosition(sf::Vector2i(x, y), window);

			draw(grid, x / cell_size, y / cell_size, 1);
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			int x = sf::Mouse::getPosition(window).x;
			int y = sf::Mouse::getPosition(window).y;

			if (x < 0)
				x = 0;
			if (y < 0)
				y = 0;
			if (x > width)
				x = width;
			if (y > height)
				y = height;
			sf::Mouse::setPosition(sf::Vector2i(x, y), window);

			draw(grid, x / cell_size, y / cell_size, 0);
		}

		for (int row = 0; row < rows; ++row)
		{
			for (int col = 0; col < cols; ++col)
			{
				float x = cell_size * col, y = cell_size * row;
				Cell cell = grid[row][col];
				if (cell.is_alive())
				{
					cells.append(
						sf::Vertex(sf::Vector2f(x, y), cellColor[cell.get_age()]));
					cells.append(sf::Vertex(sf::Vector2f(x + cell_size, y),
											cellColor[cell.get_age()]));
					cells.append(sf::Vertex(sf::Vector2f(x + cell_size, y + cell_size),
											cellColor[cell.get_age()]));
					cells.append(sf::Vertex(sf::Vector2f(x, y + cell_size),
											cellColor[cell.get_age()]));
				}
				int sum = count_neighbors(grid, row, col);
				nextGen[row][col] = cell;
				if (!pause)
				{
					if (cell.is_alive())
					{
						// Any live Cell with more than three live neighbours dies, as if by
						// overpopulation.
						if (sum > 3)
							nextGen[row][col].kill();
						// Any live Cell with two or three live neighbours lives on to the
						// next generation.
						else if (sum >= 2)
							nextGen[row][col].grow();
						// Any live Cell with fewer than two live neighbours dies, as if by
						// underpopulation.
						else
							nextGen[row][col].kill();
						// Any dead Cell with exactly three live neighbours becomes a live
						// Cell, as if by reproduction.
					}
					else
					{
						if (sum == 3)
							nextGen[row][col].birth();
					}
				}
			}
		}
		sf::RectangleShape brush(sf::Vector2f(cell_size + (cell_size * pen * 2),
											  cell_size + (cell_size * pen * 2)));
		brush.setFillColor(brushColor);
		brush.setPosition(sf::Mouse::getPosition(window).x - cell_size * pen,
						  sf::Mouse::getPosition(window).y - cell_size * pen);

		window.draw(cells);
		window.draw(lines);
		window.draw(brush);
		window.display();
		grid = nextGen;
	}
	return 0;
}