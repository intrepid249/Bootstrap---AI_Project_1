#include "ResourceManager.h"

ResourceManager* ResourceManager::sm_instance = nullptr;
std::map<const char *, std::shared_ptr<aie::Texture>> *ResourceManager::m_textures = new std::map<const char *, std::shared_ptr<aie::Texture>>();
std::map<const char *, std::shared_ptr<aie::Font>> *ResourceManager::m_fonts = new std::map<const char *, std::shared_ptr<aie::Font>>();
std::map<const char *, std::shared_ptr<aie::Audio>> *ResourceManager::m_audios = new std::map<const char *, std::shared_ptr<aie::Audio>>();

ResourceManager * ResourceManager::create() {
	if (sm_instance == nullptr) sm_instance = new ResourceManager();
	return sm_instance;
}

void ResourceManager::destroy() {
	delete sm_instance;
	sm_instance = nullptr;
}

std::map<const char *, std::shared_ptr<aie::Texture>>& ResourceManager::getTextures() {
	return *m_textures;
}

std::map<const char *, std::shared_ptr<aie::Font>>& ResourceManager::getFonts() {
	return *m_fonts;
}

std::map<const char *, std::shared_ptr<aie::Audio>>& ResourceManager::getAudios() {
	return *m_audios;
}

ResourceManager::~ResourceManager() {
	m_textures->clear();
	m_fonts->clear();
	m_audios->clear();

	delete m_audios;
	delete m_fonts;
	delete m_textures;
}
