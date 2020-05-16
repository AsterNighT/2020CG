#include "SuperSampleMaterial.h"
#include "GameException.h"

namespace Library
{
	RTTI_DEFINITIONS(SuperSampleMaterial)

		SuperSampleMaterial::SuperSampleMaterial()
		: PostProcessingMaterial(),
		MATERIAL_VARIABLE_INITIALIZATION(SampleOffsets), MATERIAL_VARIABLE_INITIALIZATION(SampleWeights)
	{
	}

	MATERIAL_VARIABLE_DEFINITION(SuperSampleMaterial, SampleOffsets)
		MATERIAL_VARIABLE_DEFINITION(SuperSampleMaterial, SampleWeights)

		void SuperSampleMaterial::Initialize(Effect& effect)
	{
		PostProcessingMaterial::Initialize(effect);

		MATERIAL_VARIABLE_RETRIEVE(SampleOffsets)
			MATERIAL_VARIABLE_RETRIEVE(SampleWeights)
	}
}