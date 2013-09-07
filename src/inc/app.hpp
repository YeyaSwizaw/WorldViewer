#ifndef WV_APP_HPP
#define WV_APP_HPP

#include "defines.hpp"

#include <array>
#include <cstdlib>
#include <map>

#include <SFML/Graphics.hpp>

#include <wg/world.hpp>
#include <wg/noisemap.hpp>
#include <wg/tiledef.hpp>

WV_NS

class App {
public:
	App();

	void init();
	void run();

private:
	sf::RenderWindow wind;

	std::map<unsigned int, sf::Color> tileColours;
	std::vector<std::vector<sf::RectangleShape>> tiles;

	void generateWorld();
	void initWindow();

}; // class App;

WV_NS_END

#endif // WV_APP_HPP
