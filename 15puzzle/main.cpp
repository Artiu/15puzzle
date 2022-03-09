#include <SFML/Graphics.hpp>
#include <time.h>

const int BLOCK_SIZE = 100;

struct Coords {
	int x;
	int y;
};

class Game {
	int board[4][4];
	void movePuzzle(Coords puzzlePos) {
		for (int row = 0; row < 4; row++) {
			for (int column = 0; column < 4; column++) {
				if (board[row][column] == 0) {
					if (((row - 1 == puzzlePos.y || row + 1 == puzzlePos.y) && column == puzzlePos.x) || ((column - 1 == puzzlePos.x || column + 1 == puzzlePos.x) && row == puzzlePos.y)) {
						board[row][column] = board[puzzlePos.y][puzzlePos.x];
						board[puzzlePos.y][puzzlePos.x] = 0;
					}
				}
			}
		}
	}
public:
	void generateRandomBoard() {
		std::vector<int> numbersToFillBoard;
		for (int i = 0; i < 16; i++) {
			numbersToFillBoard.push_back(i);
		}
		int row = 0;
		int column = 0;
		while(numbersToFillBoard.size() > 0) {
			int randomIndex = rand() % numbersToFillBoard.size();
			board[row][column] = numbersToFillBoard[randomIndex];
			numbersToFillBoard.erase(numbersToFillBoard.begin() + randomIndex);
			column++;
			if (column == 4) {
				column = 0;
				row++;
			}
		}
	}
	void renderBoard(sf::RenderWindow& window) {
		window.clear();
		sf::Text text;
		sf::Font font;
		font.loadFromFile("JetBrainsMono.ttf");
		text.setFont(font);
		text.setCharacterSize(24);
		text.setFillColor(sf::Color::Black);
		sf::RectangleShape shape(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (board[i][j] != 0) {
					shape.setFillColor(sf::Color::Green);
					shape.setPosition(j * BLOCK_SIZE, i * BLOCK_SIZE);
					shape.setOutlineColor(sf::Color::Black);
					shape.setOutlineThickness(1);
					window.draw(shape);
					text.setString(std::to_string(board[i][j]));
					text.setPosition(sf::Vector2f(j * BLOCK_SIZE + BLOCK_SIZE / 2 - 12, i * BLOCK_SIZE + BLOCK_SIZE / 2 - 12));
					window.draw(text);
				}
			}
		}
		window.display();
	}

	void onClick(sf::RenderWindow& window, Coords mousePos) {
		const int columnIndex = mousePos.x / BLOCK_SIZE;
		const int rowIndex = mousePos.y / BLOCK_SIZE;
		if (columnIndex < 4 && columnIndex >= 0 && rowIndex < 4 && rowIndex >= 0) {
			movePuzzle(Coords{ columnIndex, rowIndex });
			renderBoard(window);
		}
	}

	bool checkIsPuzzleInCorrectOrder() {
		for (int rowIndex = 0; rowIndex < 4; rowIndex++) {
			for (int itemIndex = 0; itemIndex < 4; itemIndex++) {
				int item = board[rowIndex][itemIndex];
				if (item != (rowIndex * 4) + (itemIndex + 1)) {
					if (itemIndex == 3 && rowIndex == 3) {
						return true;
					}
				}
			}
		}
		return false;
	}
};

int main() {
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(400, 400), "15 puzzle game");
	Game game;
	game.generateRandomBoard();
	game.renderBoard(window);
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					game.onClick(window, Coords{ event.mouseButton.x, event.mouseButton.y });
					if (game.checkIsPuzzleInCorrectOrder()) {
						game.generateRandomBoard();
						game.renderBoard(window);
					}
				}
			}
		}
	}
}