#pragma once
#include <random>
#include <time.h>

std::default_random_engine generator((unsigned int)time(0));

int random(int min, int max)
{
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

int randomFloat(float min, float max)
{
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}