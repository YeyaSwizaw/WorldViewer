#ifndef WV_APP_HPP
#define WV_APP_HPP

#include "defines.hpp"

#include <array>
#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>

#include <SFML/Graphics.hpp>

#include <wg/eworld.hpp>
#include <wg/noisemap.hpp>
#include <wg/tiledef.hpp>

WV_NS

typedef std::vector<std::vector<sf::RectangleShape>> Chunk;

class App {
public:
	App();

	void init();
	void run();

private:
	sf::RenderWindow wind;

	std::map<unsigned int, sf::Color> tileColours;
	std::array<std::array<Chunk, 4>, 4> chunks;

	int xOffset, yOffset;

	void clearChunks();
	void generateWorld();
	void initWindow();

}; // class App;

WV_NS_END

#endif // WV_APP_HPP
