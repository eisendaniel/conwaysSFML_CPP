#include "Cell.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <future>

// constants/defs
const int WIDTH = 1024, HEIGHT = 1024;

// globals
int cell_size = 20;
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

int count_neighbors(std::vector<std::vector<Cell>> &grid, int row, int col)
{
	int sum = 0;
	for (int i = -1; i < 2; ++i)
	{
		for (int j = -1; j < 2; ++j)
		{
			int neighborX = (row + i + rows) % rows;
			int neighborY = (col + j + cols) % cols;
			sum += grid[neighborX][neighborY].is_alive(); //add neighbor
		}
	}
	sum -= grid[row][col].is_alive(); //remove self
	return sum;
}

std::vector<std::vector<Cell>> random_states()
{
	std::vector<std::vector<Cell>> grid;
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

void draw(std::vector<std::vector<Cell>> &grid, int x, int y, int state)
{
	for (int i = -pen; i <= pen; ++i)
	{
		for (int j = -pen; j <= pen; ++j)
		{
			grid[(y + i + rows) % rows][(x + j + cols) % cols].set_age(state);
		}
	}
}

bool mouse_in_window(int x, int y)
{
	if (x < 0)
		return false;
	if (y < 0)
		return false;
	if (x > WIDTH)
		return false;
	if (y > HEIGHT)
		return false;

	return true;
}

int main()
{
	/*Setup window and frame-rate
   * init default colors
   * */
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Conways",
							sf::Style::Close);
	window.setFramerateLimit(60);

	sf::Color one(255, 255, 255, 255);
	sf::Color two(255, 255, 255, 128);
	sf::Color three(255, 255, 255, 64);

	sf::Color cellColor[] = {sf::Color::White, one, two, three};
	sf::Color brushColor(128, 128, 128, 64);
	sf::Color backgroundColor(10, 10, 10);
	sf::Color lineColor(128, 128, 128, 64);

// restart and definition point
restart:
	cols = WIDTH / cell_size;
	rows = HEIGHT / cell_size;
	pen = rows / 50;
	float brush_size = cell_size + (cell_size * pen * 2);
	std::vector<std::vector<Cell>> grid(rows, std::vector<Cell>(cols, Cell()));
	std::vector<std::vector<Cell>> nextGen = grid;
	sf::VertexArray cells(sf::Quads);
	sf::VertexArray lines(sf::Lines);

	if (cell_size > 2)
	{
		for (int x = 0; x < cols; ++x)
		{
			lines.append(sf::Vertex(sf::Vector2f(x * cell_size, 0), lineColor));
			lines.append(
				sf::Vertex(sf::Vector2f(x * cell_size, HEIGHT - 1), lineColor));
		}
		for (int y = 0; y < rows; ++y)
		{
			lines.append(sf::Vertex(sf::Vector2f(0, y * cell_size), lineColor));
			lines.append(
				sf::Vertex(sf::Vector2f(WIDTH - 1, y * cell_size), lineColor));
		}
	}

	sf::Font font;
	if (!font.loadFromFile("../res/CONSOLA.TTF"))
	{
		std::cerr << "Failed to load font" << std::endl;
		exit(EXIT_FAILURE);
	}
	sf::Text text;
	text.setFont(font);
	text.setFillColor(cellColor[1]);
	float frame_time;

	sf::Clock clock;

	while (window.isOpen())
	{ // lifetime of program

		std::stringstream ss;
		frame_time = clock.restart().asSeconds();
		ss << "Frame Time: " << frame_time * 1000 << "ms , FPS: " << 1.f / frame_time;
		text.setString(sf::String(ss.str()));

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
					pen = 0;
				else if (d >= rows / 3)
					pen = rows / 3;
				else
					pen = d;

				brush_size = cell_size + (cell_size * pen * 2);
			}
			else if (event.type == sf::Event::MouseButtonPressed)
				window.setMouseCursorVisible(false);
			else if (event.type == sf::Event::MouseButtonReleased)
				window.setMouseCursorVisible(true);
		}

		cells.clear();
		window.clear(backgroundColor);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			int x = sf::Mouse::getPosition(window).x;
			int y = sf::Mouse::getPosition(window).y;

			if (mouse_in_window(x, y))
				draw(grid, x / cell_size, y / cell_size, 1);
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			int x = sf::Mouse::getPosition(window).x;
			int y = sf::Mouse::getPosition(window).y;

			if (mouse_in_window(x, y))
				draw(grid, x / cell_size, y / cell_size, 0);
		}

		for (int row = 0; row < rows; ++row)
		{
			for (int col = 0; col < cols; ++col)
			{
				Cell &cell = grid[row][col];

				float x = cell_size * col, y = cell_size * row;
				if (grid[row][col].is_alive())
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
				nextGen[row][col].set_age(cell.get_age());

				if (!pause)
				{
					int sum = count_neighbors(grid, row, col);
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
		sf::RectangleShape brush(sf::Vector2f(brush_size, brush_size));
		brush.setFillColor(brushColor);
		brush.setPosition(sf::Mouse::getPosition(window).x - brush_size / 2.0,
						  sf::Mouse::getPosition(window).y - brush_size / 2.0);

		window.draw(cells);
		window.draw(lines);
		window.draw(brush);
		window.draw(text);
		window.display();
		grid = nextGen;
	}
	return 0;
}