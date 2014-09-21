#pragma once
#include <algorithm>
#include <random>

#include "BenchmarkBase.h"

#include "properties/Amplitude.h"

#include <mathutils/math_util.h>

/**
 *
 *  Pan : varie entre -1 et 1.
 *  -1 : tout à gauche.
 *   1 : tout à droite.
 *
 * Cas stéréo : -1 : canal gauche à 1, canal droit à 0.
 *				 1 : canal gauche à 0, canal droit à 1.
 */
template <typename data_type>
class Pan : public BenchmarkBase<data_type>
{
		using BenchmarkBase<data_type>::conf;
		using size_type = typename Parameters<data_type>::size_type;

	public:
		Pan(const Parameters<data_type>& configuration):
			BenchmarkBase<data_type>("Pan", configuration)
		{
		}

		virtual void operator()(Audio_p& data) override
		{
			auto& channelsData = getAudio<data_type>(data);

			// Left
			MathUtil::apply(channelsData[0],
					[&] (data_type x)
			{
				return x * (-0.5 * _pan + 0.5);
			});

			// Right
			MathUtil::apply(channelsData[1],
					[&] (data_type x)
			{
				return x * (0.5 * _pan + 0.5);
			});
		}

		void setPan(double p)
		{
			_pan = p;
		}

	private:
		volatile double _pan{0};
};
