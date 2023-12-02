#pragma once
#include <numeric>

class Random
{
public:
    static void Init();

    // Generate a random int
    static int GenerateInt32();

    // Generate a random int between min and max
    static int GenerateInt32(int min, int max);

    // Generate a random float between 0 and 1
    static float GenerateFloat();

    // Generate a random float between min and max
    static float GenerateFloat(float min, float max);
};
