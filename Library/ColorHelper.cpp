#include "ColorHelper.h"

std::random_device ColorHelper::sDevice;
std::default_random_engine ColorHelper::sGenerator(sDevice());
std::uniform_real_distribution<float> ColorHelper::sDistribution(0, 1);

const vec4 ColorHelper::Black = { 0.0f, 0.0f, 0.0f, 1.0f };
const vec4 ColorHelper::White = { 1.0f, 1.0f, 1.0f, 1.0f };
const vec4 ColorHelper::Red = { 1.0f, 0.0f, 0.0f, 1.0f };
const vec4 ColorHelper::Green = { 0.0f, 1.0f, 0.0f, 1.0f };
const vec4 ColorHelper::Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
const vec4 ColorHelper::Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
const vec4 ColorHelper::BlueGreen = { 0.0f, 1.0f, 1.0f, 1.0f };
const vec4 ColorHelper::Purple = { 1.0f, 0.0f, 1.0f, 1.0f };
const vec4 ColorHelper::CornflowerBlue = { 0.392f, 0.584f, 0.929f, 1.0f };
const vec4 ColorHelper::Wheat = { 0.961f, 0.871f, 0.702f, 1.0f };
const vec4 ColorHelper::LightGray = { 0.5f, 0.5f, 0.5f, 1.0f };

vec4 ColorHelper::RandomColor() {
	float r = sDistribution(sGenerator);
	float g = sDistribution(sGenerator);
	float b = sDistribution(sGenerator);

	return vec4(r, g, b, 1.0f);
}

vec4 ColorHelper::ToFloat4(const Color& color) {
	return vec4(color.r, color.g, color.b, color.a);
}

Color::Color() :r(0), b(0), g(0), a(0) {
}

Color::Color(float r, float b, float g, float a) : r(r), g(g), b(b), a(a) {
}
