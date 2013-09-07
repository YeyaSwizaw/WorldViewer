#include "inc/app.hpp"

WV_NS

App::App() {
	srand(time(NULL));

} // App::App();


void App::init() {
	generateWorld();
	initWindow();

} // void App::init();

void App::generateWorld() {
	wg::World::Ptr w = wg::World::create()->setChunkSize(WV_CHUNK_SIZE, WV_CHUNK_SIZE);

	tileColours.clear();
	tiles.clear();

	wg::NoiseMap::Ptr heightmap = wg::NoiseMap::create()
		->setSeed(std::to_string(rand()))
		->setGridSize(0.025);

	wg::NoiseMap::Ptr random = wg::NoiseMap::create()
		->setSeed(std::to_string(rand()))
		->setGridSize(0.1);

	wg::NoiseMap::Ptr climate = wg::NoiseMap::create()
		->setSeed(std::to_string(rand()))
		->setGridSize(0.03);

	wg::TileDef::Ptr tileWater = w->newTile()
		->addConstraint(heightmap < -0.3);

	wg::TileDef::Ptr tileBeach = w->newTile()
		->addConstraint(heightmap < -0.2);

	wg::TileDef::Ptr tileHighMnt = w->newTile()
		->addConstraint(heightmap > 0.6)
		->addConstraint(random > 0.2);

	wg::TileDef::Ptr tileMnt = w->newTile()
		->addConstraint(heightmap > 0.45);

	wg::TileDef::Ptr tileForest = w->newTile()
		->addConstraint(climate > 0.2)
		->addConstraint(heightmap > -0.1);

	wg::TileDef::Ptr tileDesert = w->newTile()
		->addConstraint(climate < -0.45)
		->addConstraint(heightmap > -0.1);

	wg::TileDef::Ptr tilePlains = w->newTile();

	tileColours.insert({tileWater->getId(), sf::Color(0, 255, 255)});
	tileColours.insert({tileBeach->getId(), sf::Color(255, 200, 0)});
	tileColours.insert({tileMnt->getId(), sf::Color(180, 180, 180)});
	tileColours.insert({tileHighMnt->getId(), sf::Color(250, 250, 250)});
	tileColours.insert({tileForest->getId(), sf::Color(0, 128, 0)});
	tileColours.insert({tileDesert->getId(), sf::Color(255, 200, 0)});
	tileColours.insert({tilePlains->getId(), sf::Color(0, 255, 0)});

	for(auto& row : w->generate(0, 0)->getMap()) {
		tiles.push_back(std::vector<sf::RectangleShape>());

		for(auto& id : row) {
			tiles.back().push_back(sf::RectangleShape(sf::Vector2f(WV_TILE_SIZE, WV_TILE_SIZE)));
			tiles.back().back().setPosition((tiles.back().size() - 1) * WV_TILE_SIZE, (tiles.size() - 1) * WV_TILE_SIZE);
			tiles.back().back().setFillColor(tileColours[id]);

		} // for(auto& id : row);

	} // for(auto& row : w->generate(0, 0)->getMap());

} // void App::generateWorld();

void App::initWindow() {
	wind.create(sf::VideoMode(640, 640), WV_TITLE);

} // void App::initWindow();


void App::run() {
	while(wind.isOpen()) {
		sf::Event e;
		while(wind.pollEvent(e)) {
			if(e.type == sf::Event::Closed) {
				wind.close();

			} // if(e.type == sf::Event::Closed);

			if(e.type == sf::Event::KeyPressed) {
				if(e.key.code == sf::Keyboard::Space) {
					generateWorld();

				} // if(e.key.code = sf::Keyboard::Space);

			} // if(e.type = sf::Event::KeyPressed);

		} // while(wind.pollEvent(e));

		wind.clear();

		for(auto& row : tiles) {
			for(auto& t : row) {
				wind.draw(t);

			} // for(auto& t : row);

		} // for(auto& row : tiles);

		wind.display();

	} // while(wind.isOpen());

} // void App::run();

WV_NS_END
