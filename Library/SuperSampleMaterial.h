#pragma once

#include "Common.h"
#include "PostProcessingMaterial.h"

namespace Library
{
	class SuperSampleMaterial : public PostProcessingMaterial
	{
		RTTI_DECLARATIONS(PostProcessingMaterial, SuperSampleMaterial)

			MATERIAL_VARIABLE_DECLARATION(SampleOffsets)
			MATERIAL_VARIABLE_DECLARATION(SampleWeights)

	public:
		SuperSampleMaterial();

		virtual void Initialize(Effect& effect) override;
	};
}