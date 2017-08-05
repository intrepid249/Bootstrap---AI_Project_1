#include "PixelTestingApp.h"

int main() {
	
	auto app = new PixelTestingApp();
	app->run("AIE", 640, 480, false);
	delete app;

	return 0;
}