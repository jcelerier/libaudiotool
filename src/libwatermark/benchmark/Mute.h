#pragma once
#include <algorithm>
#include <random>

#include "BenchmarkBase.h"

#include "../mathutils/math_util.h"

template <typename data_type>
class Mute : public BenchmarkBase<data_type>
{
		using BenchmarkBase<data_type>::conf;
		using size_type = typename Parameters<data_type>::size_type;

	public:
		Mute(const Parameters<data_type>& configuration):
			BenchmarkBase<data_type>("Mute", configuration)
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
					return _mute? 0 : x;
				});
			}
		}

		void mute()
		{
			_mute = true;
		}

		void unmute()
		{
			_mute = false;
		}

	private:
		volatile bool _mute{false};
};
