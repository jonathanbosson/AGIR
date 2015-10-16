#include "RNG.h"


RNG::RNG(void)
{
    mt = std::mt19937(rd());
    dist = std::uniform_real_distribution<float>(0.0f, 1.0f);
}


RNG::~RNG(void)
{
}
