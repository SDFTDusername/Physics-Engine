#pragma once
#include <random>
#include <time.h>

std::default_random_engine generator((unsigned int)time(0));

int random(const int min, const int max)
{
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

float randomFloat(const float min, const float max)
{
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}