#include "pch.h"

std::random_device rd;
std::uniform_real_distribution<float> random_color(0.0f, 1.0f);
std::uniform_real_distribution<float> random_size(0.07f, 0.15f);
std::uniform_real_distribution<float> random_dir(-0.01f, 0.01f);
std::uniform_real_distribution<float> random_speed(0.005f, 0.02f);
