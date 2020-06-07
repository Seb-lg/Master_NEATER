//
// Created by seb on 12/02/19.
//

#include <SFML/Window/Event.hpp>
#include <chrono>
#include <thread>
#include <iostream>
#include "Graphical.hpp"

Graphical::Graphical(int size, int width):
	window(sf::VideoMode(width * 2, width), "AI Battle", sf::Style::Close, sf::ContextSettings(0, 0, 10)),
	widthMap(size){

	widthCircle = static_cast<float>(width) / (size + 2);

	circle.setRadius(widthCircle/2 - 1.f);
	circle.setFillColor(sf::Color::Black);
	//circle.setOutlineThickness(1.f);
	//circle.setOutlineColor(sf::Color(211,211,211,255));
}

void Graphical::draw(std::vector<char> tableau, int width) {
	int i;

	i = width / 2 + 3;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < width; y++) {
			if (tableau[y*width + x] == 'S')
				circle.setFillColor(sf::Color::Green);
			else if (tableau[y*width + x] == 'A')
				circle.setFillColor(sf::Color::Red);
			else
				circle.setFillColor(sf::Color(50, 50, 50, 255));
			circle.setPosition((i + x) * widthCircle, (y + 1) * widthCircle);
			window.draw(circle);
		}
	}
}

void Graphical::update() {
	sf::Event event;

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window.close();
	}
	window.display();
	window.clear();
}