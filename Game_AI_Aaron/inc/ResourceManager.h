#pragma once
#include <map>
#include <memory>

namespace aie {
	class Texture;
	class Font;
	class Audio;
}

/** Singleton class to manage shared resources across the global scope of the program
* Written in collaberation with Sebastian Toy
* @author Jack McCall*/
class ResourceManager {
public:
	/** Initialise the resource manager singleton - must be matched with a call to destroy()*/
	static ResourceManager *create();
	/** Cleanup memory for the resource manager*/
	static void destroy();

	/** Return a reference to the map of texture data*/
	static std::map<const char *, std::shared_ptr<aie::Texture>> &getTextures();
	/** Return a reference to the map of font data*/
	static std::map<const char *, std::shared_ptr<aie::Font>> &getFonts();
	/** Return a reference to the map of audio data*/
	static std::map<const char *, std::shared_ptr<aie::Audio>> &getAudios();

private:
	ResourceManager() = default;
	~ResourceManager();

	static std::map<const char *, std::shared_ptr<aie::Texture>> *m_textures;
	static std::map<const char *, std::shared_ptr<aie::Font>> *m_fonts;
	static std::map<const char *, std::shared_ptr<aie::Audio>> *m_audios;

	static ResourceManager *sm_instance;
};