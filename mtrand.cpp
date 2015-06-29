#include "mtrand.h"

namespace RandGen
{

	MtRand::MtRand()
	{
		std::random_device rnd;
		mt.seed( rnd() );
	}


	MtUniformRand& MtUniformRand::GetInstance()
	{
		static MtUniformRand rand;
		return rand;
	}


	MtNormalRand& MtNormalRand::GetInstance()
	{
		static MtNormalRand rand;
		return rand;
	}


	MtExponentialRand& MtExponentialRand::GetInstance()
	{
		static MtExponentialRand rand;
		return rand;
	}

	MtParetoRand& MtParetoRand::GetInstance()
	{
		static MtParetoRand rand;
		return rand;
	}

}
