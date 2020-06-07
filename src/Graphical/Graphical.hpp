//
// Created by seb on 12/02/19.
//

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

class Graphical {
public:
	Graphical() = delete;

	Graphical(int size, int width);

	void draw(std::vector<char> tableau, int width);

	void update();

	inline bool isOpen() {
		return window.isOpen();
	}

private:
	sf::RenderWindow 	window;
	sf::CircleShape		circle;
	int 			widthMap;
	float 			widthCircle;
};

