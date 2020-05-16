#pragma once

#include "Common.h"
#include "GameComponent.h"

namespace Library
{
	class Light : public GameComponent
	{
		RTTI_DECLARATIONS(Light, GameComponent)

	public:
		Light(Game& game);
		virtual ~Light();

		const Color& Color() const;
		vec4 ColorVector() const;
		void SetColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a);
		void SetColor(Color color);
		void SetColor(Fvec4 color);

	protected:
		Color mColor;
	};
}

