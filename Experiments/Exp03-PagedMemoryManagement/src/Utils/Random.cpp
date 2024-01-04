#include "Random.h"
#include <random>
#include <chrono>

static std::mt19937 s_RandomEngine;
static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;

void Random::Init()
{
    s_RandomEngine.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

int Random::GenerateInt32()
{
    return s_Distribution(s_RandomEngine);
}

int Random::GenerateInt32(int min, int max)
{
    if(min == max)
        return min;

    if (min > max)
        std::swap(min, max);

    return min + s_Distribution(s_RandomEngine) % (max - min);
}

float Random::GenerateFloat()
{
    return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
}

float Random::GenerateFloat(float min, float max)
{
    return min + GenerateFloat() * (max - min);
}

