#include "PixelTestingApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"

PixelTestingApp::PixelTestingApp() {

}

PixelTestingApp::~PixelTestingApp() {

}

bool PixelTestingApp::startup() {
	
	m_2dRenderer = new aie::Renderer2D();
	m_font = new aie::Font("./font/consolas.ttf", 32);

	m_image = new aie::Texture("./textures/Endless/testImage.png");

	return true;
}

void PixelTestingApp::shutdown() {
	delete m_image;
	delete m_font;
	delete m_2dRenderer;
}

void PixelTestingApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();


}

void PixelTestingApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	m_2dRenderer->drawSprite(m_image, 0, 0, 0, 0, 0, 0, 0, 0);


	aie::Input *input = aie::Input::getInstance();
	int mx, my;
	input->getMouseXY(&mx, &my);

	int xi = mx, yi = getWindowHeight() - my;
	unsigned int r = m_image->getPixels()[(yi * m_image->getWidth() + xi) * 4 + 0];
	unsigned int g = m_image->getPixels()[(yi * m_image->getWidth() + xi) * 4 + 1];
	unsigned int b = m_image->getPixels()[(yi * m_image->getWidth() + xi) * 4 + 2];
	unsigned int a = m_image->getPixels()[(yi * m_image->getWidth() + xi) * 4 + 3];
	
	// output some text, uses the last used colour
	char buffer[255];
	sprintf_s(buffer, "R: %3d, G: %3d, B: %3d, A: %3d", r, g, b, a);
	m_2dRenderer->drawText(m_font, buffer, 10, 10);

	// done drawing sprites
	m_2dRenderer->end();
}