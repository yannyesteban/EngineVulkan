#pragma once
#include <vector>
class Shader
{
public:
	const char* id;
	const char* source;
	const char* type;
	const char* entry;
	std::vector<int> layout;
};

