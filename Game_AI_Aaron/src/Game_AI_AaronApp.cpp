#include "Game_AI_AaronApp.h"
#include <Renderer2D.h>
#include <Texture.h>
#include <Font.h>
#include <Input.h>
#include <Entities\GameObject.h>

Game_AI_AaronApp::Game_AI_AaronApp() {

}

Game_AI_AaronApp::~Game_AI_AaronApp() {

}

bool Game_AI_AaronApp::startup() {
	
	m_renderer = new aie::Renderer2D();
	m_font = new aie::Font("./font/consolas.ttf", 32);

	m_player = std::unique_ptr<GameObject>(new GameObject());
	m_player->setPos(glm::vec2(getWindowWidth() * 0.5f, getWindowHeight() * 0.5f));

	return true;
}

void Game_AI_AaronApp::shutdown() {
	delete m_font;
	delete m_renderer;
}

void Game_AI_AaronApp::update(float deltaTime) {

	m_player->applyForce(glm::vec2(10, 0));
	m_player->update(deltaTime);
	m_player->wrapScreenBounds();

	// input example
	aie::Input* input = aie::Input::getInstance();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Game_AI_AaronApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_renderer->begin();

	m_player->render(m_renderer);
	
	// output some text, uses the last used colour
	m_renderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_renderer->end();
}