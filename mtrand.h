// 乱数生成クラス（シングルトン）
// C++11以上必要

#pragma once

#include <functional>
#include <random>

namespace RandGen
{
	//========================================
	//           乱数生成基底クラス
	//========================================
	class MtRand
	{
		protected:
			//mersenne_twister_engine（パラメータ設定済み）のtypedef
			std::mt19937_64 mt;
			MtRand(); //生成禁止
	};


	//========================================
	//               一様分布
	//========================================
	class MtUniformRand : public MtRand
	{
		public:
			int operator()(int min, int max);
			double operator()(double min, double max);
			static MtUniformRand& GetInstance();
	};


	inline int MtUniformRand::operator()(int min, int max)
	{
		std::uniform_int_distribution<int> dist(min, max);
		return dist(mt);
	}


	inline double MtUniformRand::operator()(double min, double max)
	{
		std::uniform_real_distribution<double> dist(min, max);
		return dist(mt);
	}

	const auto unifi = [&](int min, int max)		{ return MtUniformRand::GetInstance()(min, max);   };
	const auto unifd = [&](double min, double max)	{ return MtUniformRand::GetInstance()(min, max);   };


	//========================================
	//               正規分布
	//========================================
	class MtNormalRand : public MtRand
	{
		public:
			double operator()(double mean, double sd);
			static MtNormalRand& GetInstance();
	};


	inline double MtNormalRand::operator()(double mean, double sd)
	{
		std::normal_distribution<> dist(mean, sd);
		return dist(mt);
	}

	const auto nml   = [&](double mean, double sd)	{ return MtNormalRand::GetInstance()(mean, sd);    };


	//========================================
	//               指数分布
	//========================================
	class MtExponentialRand : public MtRand
	{
		public:
			double operator()(double lambda);
			static MtExponentialRand& GetInstance();
	};


	inline double MtExponentialRand::operator()(double lambda)
	{
		std::exponential_distribution<> dist(lambda);
		return dist(mt);
	}

	const auto exp = [&](double lambda) { return MtExponentialRand::GetInstance()(lambda); };


	//========================================
	//              パレート分布
	//========================================
	class MtParetoRand : public MtRand
	{
		public:
			double operator()(double a, double b);
			static MtParetoRand& GetInstance();
	};


	inline double MtParetoRand::operator()(double a, double b)
	{
		return b / pow(1 - unifd(0, 1), 1/a);
	}

	const auto prt = [&](double a, double b) { return MtParetoRand::GetInstance()(a, b); };

}// namespace RandGen