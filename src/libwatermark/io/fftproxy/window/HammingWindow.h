#pragma once

#include "WindowBase.h"

template<typename data_type>
class HammingWindow: public WindowBase<data_type>
{
		using size_type = typename Parameters<data_type>::size_type;
	public:
		HammingWindow(const Parameters<data_type>& cfg):
			WindowBase<data_type>(cfg)
		{
		}

		virtual void apply(std::vector<data_type>& v, const size_type increment) final override
		{
			const double f = 2.0 * M_PI / increment;
			for(auto i = 0U; i < increment; ++i)
			{
				v[i] *= 0.54 - 0.46 * cos(i * f);
			}
		}
};