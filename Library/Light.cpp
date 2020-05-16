#include "Light.h"
#include "ColorHelper.h"

namespace Library
{
	RTTI_DEFINITIONS(Light)

	Light::Light(Game& game)
		: GameComponent(game), mColor(reinterpret_cast<const float*>(&ColorHelper::White))
	{
	}

	Light::~Light()
	{
	}

	const Color& Light::Color() const
	{
		return mColor;
	}

	vec4 Light::ColorVector() const
	{
		return XMLoadColor(&mColor);
	}

	void Light::SetColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
	{
		Color color(r, g, b, a);
		SetColor(color);
	}

	void Light::SetColor(Color color)
	{
		mColor = color;
	}

	void Light::SetColor(Fvec4 color)
	{
		XMStoreColor(&mColor, color);
	}
}