#pragma once
#include <algorithm>
#include <random>

#include "BenchmarkBase.h"

#include "properties/Amplitude.h"
#include "../mathutils/math_util.h"

template <typename data_type>
class Amplify : public BenchmarkBase<data_type>, public AmplitudeProperty
{
		
		using size_type = typename Parameters<data_type>::size_type;

	public:
		using BenchmarkBase<data_type>::conf;
		Amplify(const Parameters<data_type>& configuration):
			BenchmarkBase<data_type>("Amplify", configuration)
		{
		}

		virtual void operator()(Audio_p& data) override
		{
			auto& channelsData = getAudio<data_type>(data);

			for(auto& sampleData : channelsData)
			{
				MathUtil::apply(sampleData,
					  [&] (data_type x)
				{
					return x * _amplitude;
				});
			}
		}

		void setGain(double g)
		{
			_amplitude = g;
		}

	private:
		volatile double _amplitude{1.0};
};
