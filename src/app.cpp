#include "inc/app.hpp"

WV_NS

App::App() {
	srand(time(NULL));

} // App::App();


void App::init() {
	generateWorld();
	initWindow();

} // void App::init();

void App::clearChunks() {
	for(int i = 0; i < 6; ++i) {
		chunks[i].fill(Chunk());

	} // for(int i = 0; i < 3; ++i);

} // void App::clearChunks();

void App::generateWorld() {
	std::cout << "Generating New World: " << std::endl;

	w = (new wg::EnhancedWorld<sf::Color>)->setChunkSize(WV_CHUNK_SIZE);

	clearChunks();
	xOffset = 0;
	yOffset = 0;
	xChunkOff = 0;
	yChunkOff = 0;

	wg::RandomNoiseMap* heightmap = w->addRandomNoiseMap()
		->setSeed(std::to_string(rand()))
		->setGridSize(0.025);

	wg::RandomNoiseMap* random = w->addRandomNoiseMap()
		->setSeed(std::to_string(rand()))
		->setGridSize(0.1);

	wg::RandomNoiseMap* climate = w->addRandomNoiseMap()
		->setSeed(std::to_string(rand()))
		->setGridSize(0.03);

	// Water
	w->addTileDefinition(sf::Color(0, 255, 255))
		->addConstraint({heightmap, wg::ConstraintType::LT, -0.3});

	// Beach
	w->addTileDefinition(sf::Color(255, 200, 0))
		->addConstraint({heightmap, wg::ConstraintType::LT, -0.2});

	// High Mountains
	w->addTileDefinition(sf::Color(250, 250, 250))
		->addConstraint({heightmap, wg::ConstraintType::GT, 0.6})
		->addConstraint({random, wg::ConstraintType::GT, 0.2});

	// Mountains
	w->addTileDefinition(sf::Color(180, 180, 180))
		->addConstraint({heightmap, wg::ConstraintType::GT, 0.45});

	// Forest
	w->addTileDefinition(sf::Color(0, 128, 0))
		->addConstraint({heightmap, wg::ConstraintType::GT, -0.1})
		->addConstraint({climate, wg::ConstraintType::GT, 0.2});

	// Desert
	w->addTileDefinition(sf::Color(255, 200, 0))
		->addConstraint({heightmap, wg::ConstraintType::GT, -0.1})
		->addConstraint({climate, wg::ConstraintType::LT, -0.45});

	// Plains
	w->addTileDefinition(sf::Color(0, 255, 0));

	for(int chunkX = 0; chunkX < 6; ++chunkX) {
		for(int chunkY = 0; chunkY < 6; ++chunkY) {
			std::cout << "	Generating Chunk (" << chunkX - 1 << ", " << chunkY - 1 << ")" << std::endl;

			int x = chunkX - 1, y = chunkY - 1;
			int tileX = 0, tileY = 0;
			for(auto& row : w->generate(x, y)->getObjects()) {
				chunks[chunkY][chunkX].push_back(std::vector<sf::RectangleShape>());

				tileX = 0;

				for(sf::Color& colour : row) {
					chunks[chunkY][chunkX].back().push_back(sf::RectangleShape(sf::Vector2f(WV_TILE_SIZE, WV_TILE_SIZE)));

					chunks[chunkY][chunkX].back().back().setPosition((x * WV_TILE_SIZE * WV_CHUNK_SIZE) + (tileX * WV_TILE_SIZE), 
							(y * WV_TILE_SIZE * WV_CHUNK_SIZE) + (tileY * WV_TILE_SIZE));

					chunks[chunkY][chunkX].back().back().setFillColor(colour);

					tileX++;

				} // for(sf::Color& colour : row);

				tileY++;

			} // for(auto& row : w->generate(chunkX - 1, chunkY - 1)->getObjects());

		} // for(int chunkY = 0; chunkY < 3; ++chunkY);

	} // for(int chunkX = 0; chunkX < 3; ++chunkX);

	std::cout << "Done!" << std::endl;

} // void App::generateWorld();

void App::initWindow() {
	wind.create(sf::VideoMode(WV_CHUNK_SIZE * WV_TILE_SIZE * 4, WV_CHUNK_SIZE * WV_TILE_SIZE * 4), WV_TITLE);

} // void App::initWindow();

void App::scrollWorld(int x, int y) {
	xOffset += x * WV_SCROLL_SPEED;
	yOffset += y * WV_SCROLL_SPEED;

	std::cout << "Offset: (" << xOffset << ", " << yOffset << ")" << std::endl;

	for(auto& chunkRow : chunks) {
		for(auto& chunk : chunkRow) {
			for(auto& row : chunk) {
				for(auto& t : row) {
					t.move(x * WV_SCROLL_SPEED, y * WV_SCROLL_SPEED);

				} // for(auto& t : row);

			} // for(auto& row : chunks[chunkY][chunkX]);

		} // for(auto& chunk : chunkRow);

	} // for(auto& chunkRow : chunks);

	// Generate new chunks if neccesary
	if(xOffset < 0 - ((WV_CHUNK_SIZE * WV_TILE_SIZE) / 2)) {
		xOffset += WV_CHUNK_SIZE * WV_TILE_SIZE;
		xChunkOff += 1;

		for(int chunkY = 0; chunkY < 6; ++chunkY) {
			for(int chunkX = 0; chunkX < 5; ++chunkX) {
				for(int tileY = 0; tileY < WV_CHUNK_SIZE; ++tileY) {
					for(int tileX = 0; tileX < WV_CHUNK_SIZE; ++tileX) {
						chunks[chunkY][chunkX][tileY][tileX].move(WV_CHUNK_SIZE * WV_TILE_SIZE, 0);
						chunks[chunkY][chunkX][tileY][tileX].setFillColor(chunks[chunkY][chunkX+1][tileY][tileX].getFillColor());

					} // for(int tileX = 0; tileX < WV_CHUNK_SIZE; ++tileX);

				} // for(int tileY = 0; tileY < WV_CHUNK_SIZE; ++tileY);

			} // for(int chunkX = 0; chunkX < 5; ++chunkX);

			std::cout << "Generating Chunk (" << 4 + xChunkOff << ", " << (chunkY - 1) + yChunkOff << ")" << std::endl;
			w->generate(4 + xChunkOff, (chunkY - 1) + yChunkOff);

			for(int tileY = 0; tileY < WV_CHUNK_SIZE; ++tileY) {
				for(int tileX = 0; tileX < WV_CHUNK_SIZE; ++tileX) {
						chunks[chunkY][5][tileY][tileX].move(WV_CHUNK_SIZE * WV_TILE_SIZE, 0);
						chunks[chunkY][5][tileY][tileX].setFillColor(w->getObject(tileX, tileY));

				} // for(int tileX = 0; tileX < WV_CHUNK_SIZE; ++tileX);

			} // for(int tileY = 0; tileY < WV_CHUNK_SIZE; ++tileY);

		} // for(int chunkY = 0; chunkY < 6; ++chunkY);

	} // if(xOffset < -(WV_CHUNK_SIZE * WV_TILE_SIZE / 2));
	else if(xOffset > ((WV_CHUNK_SIZE * WV_TILE_SIZE) / 2)) {
		xOffset -= WV_CHUNK_SIZE * WV_TILE_SIZE;
		xChunkOff -= 1;

		for(int chunkY = 5; chunkY >= 0; --chunkY) {
			for(int chunkX = 5; chunkX >= 1; --chunkX) {
				for(int tileY = 0; tileY < WV_CHUNK_SIZE; ++tileY) {
					for(int tileX = 0; tileX < WV_CHUNK_SIZE; ++tileX) {
						chunks[chunkY][chunkX][tileY][tileX].move(0 - (WV_CHUNK_SIZE * WV_TILE_SIZE), 0);
						chunks[chunkY][chunkX][tileY][tileX].setFillColor(chunks[chunkY][chunkX-1][tileY][tileX].getFillColor());

					} // for(int tileX = 0; tileX < WV_CHUNK_SIZE; ++tileX);

				} // for(int tileY = 0; tileY < WV_CHUNK_SIZE; ++tileY);

			} // for(int chunkX = 5; chunkX >= 1; --chunkX);

			std::cout << "Generating Chunk (" << -1 + xChunkOff << ", " << (chunkY - 1) + yChunkOff << ")" << std::endl;
			w->generate(-1 + xChunkOff, (chunkY - 1) + yChunkOff);

			for(int tileY = 0; tileY < WV_CHUNK_SIZE; ++tileY) {
				for(int tileX = 0; tileX < WV_CHUNK_SIZE; ++tileX) {
						chunks[chunkY][0][tileY][tileX].move(0 - (WV_CHUNK_SIZE * WV_TILE_SIZE), 0);
						chunks[chunkY][0][tileY][tileX].setFillColor(w->getObject(tileX, tileY));

				} // for(int tileX = 0; tileX < WV_CHUNK_SIZE; ++tileX);

			} // for(int tileY = 0; tileY < WV_CHUNK_SIZE; ++tileY);

		} // for(int chunkY = 5; chunkY >= 0; --chunkY);

	} // else if(xOffset > ((WV_CHUNK_SIZE * WV_TILE_SIZE) / 2));
	else if(yOffset < 0 - ((WV_CHUNK_SIZE * WV_TILE_SIZE) / 2)) {
		yOffset += WV_CHUNK_SIZE * WV_TILE_SIZE;
		yChunkOff += 1;

		for(int chunkX = 0; chunkX < 6; ++chunkX) {
			for(int chunkY = 0; chunkY < 5; ++chunkY) {
				for(int tileY = 0; tileY < WV_CHUNK_SIZE; ++tileY) {
					for(int tileX = 0; tileX < WV_CHUNK_SIZE; ++tileX) {
						chunks[chunkY][chunkX][tileY][tileX].move(0, WV_CHUNK_SIZE * WV_TILE_SIZE);
						chunks[chunkY][chunkX][tileY][tileX].setFillColor(chunks[chunkY+1][chunkX][tileY][tileX].getFillColor());

					} // for(int tileX = 0; tileX < WV_CHUNK_SIZE; ++tileX);

				} // for(int tileY = 0; tileY < WV_CHUNK_SIZE; ++tileY);

			} // for(int chunkX = 0; chunkX < 5; ++chunkX);

			std::cout << "Generating Chunk (" << (chunkX - 1) + xChunkOff << ", " << 4 + yChunkOff << ")" << std::endl;
			w->generate((chunkX - 1) + xChunkOff, 4 + yChunkOff);

			for(int tileY = 0; tileY < WV_CHUNK_SIZE; ++tileY) {
				for(int tileX = 0; tileX < WV_CHUNK_SIZE; ++tileX) {
						chunks[5][chunkX][tileY][tileX].move(0, WV_CHUNK_SIZE * WV_TILE_SIZE);
						chunks[5][chunkX][tileY][tileX].setFillColor(w->getObject(tileX, tileY));

				} // for(int tileX = 0; tileX < WV_CHUNK_SIZE; ++tileX);

			} // for(int tileY = 0; tileY < WV_CHUNK_SIZE; ++tileY);

		} // for(int chunkY = 0; chunkY < 6; ++chunkY);

	} // if(xOffset < -(WV_CHUNK_SIZE * WV_TILE_SIZE / 2));
	else if(yOffset > ((WV_CHUNK_SIZE * WV_TILE_SIZE) / 2)) {
		yOffset -= WV_CHUNK_SIZE * WV_TILE_SIZE;
		yChunkOff -= 1;

		for(int chunkX = 5; chunkX >= 0; --chunkX) {
			for(int chunkY = 5; chunkY >= 1; --chunkY) {
				for(int tileY = 0; tileY < WV_CHUNK_SIZE; ++tileY) {
					for(int tileX = 0; tileX < WV_CHUNK_SIZE; ++tileX) {
						chunks[chunkY][chunkX][tileY][tileX].move(0, 0 - (WV_CHUNK_SIZE * WV_TILE_SIZE));
						chunks[chunkY][chunkX][tileY][tileX].setFillColor(chunks[chunkY-1][chunkX][tileY][tileX].getFillColor());

					} // for(int tileX = 0; tileX < WV_CHUNK_SIZE; ++tileX);

				} // for(int tileY = 0; tileY < WV_CHUNK_SIZE; ++tileY);

			} // for(int chunkX = 5; chunkX >= 1; --chunkX);

			std::cout << "Generating Chunk (" << (chunkX - 1) + xChunkOff << ", " << -1 + yChunkOff << ")" << std::endl;
			w->generate((chunkX - 1) + xChunkOff, -1 + yChunkOff);

			for(int tileY = 0; tileY < WV_CHUNK_SIZE; ++tileY) {
				for(int tileX = 0; tileX < WV_CHUNK_SIZE; ++tileX) {
						chunks[0][chunkX][tileY][tileX].move(0, 0 - (WV_CHUNK_SIZE * WV_TILE_SIZE));
						chunks[0][chunkX][tileY][tileX].setFillColor(w->getObject(tileX, tileY));

				} // for(int tileX = 0; tileX < WV_CHUNK_SIZE; ++tileX);

			} // for(int tileY = 0; tileY < WV_CHUNK_SIZE; ++tileY);

		} // for(int chunkY = 5; chunkY >= 0; --chunkY);

	} // else if(xOffset > ((WV_CHUNK_SIZE * WV_TILE_SIZE) / 2));

} // void App::scrollWorld(int x, int y);

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

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			scrollWorld(-1, 0);

		} // if(sf::Keyboard::Left::isPressed());
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			scrollWorld(1, 0);

		} // else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right));

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			scrollWorld(0, -1);

		} // if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up));
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			scrollWorld(0, 1);

		} // else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down));

		wind.clear();

		for(auto& chunkRow : chunks) {
			for(auto& chunk : chunkRow) {
				for(auto& row : chunk) {
					for(auto& t : row) {
						wind.draw(t);

					} // for(auto& t : row);

				} // for(auto& row : chunks[chunkY][chunkX]);

			} // for(auto& chunk : chunkRow);

		} // for(auto& chunkRow : chunks);

		wind.display();

	} // while(wind.isOpen());

} // void App::run();

WV_NS_END
