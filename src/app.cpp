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
	wg::World* w = (new wg::World)->setChunkSize(WV_CHUNK_SIZE);

	tileColours.clear();
	tiles.clear();

	wg::RandomNoiseMap* heightmap = w->addRandomNoiseMap()
		->setSeed(std::to_string(rand()))
		->setGridSize(0.025);

	wg::RandomNoiseMap* random = w->addRandomNoiseMap()
		->setSeed(std::to_string(rand()))
		->setGridSize(0.1);

	wg::RandomNoiseMap* climate = w->addRandomNoiseMap()
		->setSeed(std::to_string(rand()))
		->setGridSize(0.03);

	wg::TileDef* tileWater = w->addTileDefinition()
		->addConstraint({heightmap, wg::ConstraintType::LT, -0.3});

	wg::TileDef* tileBeach = w->addTileDefinition()
		->addConstraint({heightmap, wg::ConstraintType::LT, -0.2});

	wg::TileDef* tileHighMnt = w->addTileDefinition()
		->addConstraint({heightmap, wg::ConstraintType::GT, 0.6})
		->addConstraint({random, wg::ConstraintType::GT, 0.2});

	wg::TileDef* tileMnt = w->addTileDefinition()
		->addConstraint({heightmap, wg::ConstraintType::GT, 0.45});

	wg::TileDef* tileForest = w->addTileDefinition()
		->addConstraint({heightmap, wg::ConstraintType::GT, -0.1})
		->addConstraint({climate, wg::ConstraintType::GT, 0.2});

	wg::TileDef* tileDesert = w->addTileDefinition()
		->addConstraint({heightmap, wg::ConstraintType::GT, -0.1})
		->addConstraint({climate, wg::ConstraintType::LT, -0.45});

	wg::TileDef* tilePlains = w->addTileDefinition();

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
