#pragma once
#include <random>
class RNG
{
public:
	RNG(void);
	~RNG(void);
	
	std::random_device rd;
    std::mt19937 mt;
    std::uniform_real_distribution<float> dist;
};

