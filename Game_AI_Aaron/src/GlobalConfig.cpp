#include <GlobalConfig.h>

ini_t* GlobalConfig::sm_instance = nullptr;

ini_t * GlobalConfig::create(const char * filename) {
	if (sm_instance == nullptr) sm_instance = new ini_t(filename, true);
	return sm_instance;
}

void GlobalConfig::destroy() {
	delete sm_instance;
	sm_instance = nullptr;
}

ini_t * GlobalConfig::getInstance() {
	return sm_instance;
}

#pragma region Constructor/Destructor
GlobalConfig::GlobalConfig() {
}

GlobalConfig::~GlobalConfig() {
}
#pragma endregion