#pragma once
#include <INI.h>
typedef INI<> ini_t;

class GlobalConfig {
public:
	static ini_t *create(const char * filename);
	static void destroy();
	static ini_t *getInstance();

private:
	GlobalConfig();
	~GlobalConfig();

	static ini_t *sm_instance;
};