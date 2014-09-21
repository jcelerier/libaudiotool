#pragma once

#include "FFTProxy.h"
#include <io/outputs/OutputManagerBase.h>
#include <io/copystyle/OutputSimple.h>
#include <io/copystyle/OutputOLA.h>
#include <mathutils/math_util.h>

template <typename data_type>
/**
 * @brief The FFTOutputProxy class
 *
 * Réalise une short time invese fourier transform
 */
class FFTOutputProxy : public FFTProxy<data_type>, public OutputManagerBase<data_type>
{
		using IOManagerBase<data_type>::channels;
		using IOManagerBase<data_type>::frames;
		using FFTProxy<data_type>::fft;
	private:
		Output_p outputImpl = nullptr;

	public:
		FFTOutputProxy(OutputManagerBase<data_type>* output,
					   FFT_p<data_type> fft_impl,
					   Parameters<data_type>& cfg):
			FFTProxy<data_type>(fft_impl, cfg),
			OutputManagerBase<data_type>(nullptr, cfg),
			outputImpl(output)
		{
		}

		FFTOutputProxy(Output_p output,
					   FFT_p<data_type> fft_impl,
					   Parameters<data_type>& cfg):
			FFTProxy<data_type>(fft_impl, cfg),
			OutputManagerBase<data_type>(nullptr, cfg),
			outputImpl(output)
		{
		}


		virtual ~FFTOutputProxy() = default;

		virtual void writeNextBuffer(Audio_p& data) final override
		{
			// 0. We put our buffer back in the FFT.
			fft->spectrum() = std::move(getSpectrum<data_type>(data));

			// 1. Perform reverse FFT
			fft->backward();

			// 2. Normalize
			if(dynamic_cast<IOManagerBase<data_type>*>(outputImpl.get())->pos() == 0) // Premier buffer
			{
				outbuff.reset(new CData<data_type>{fft->output().size(), fft->output()[0].size()});
			}

			for(auto i = 0U; i < fft->output().size(); ++i)
			{
				std::transform(fft->output()[i].begin(),
							   fft->output()[i].end(),
							   static_cast<CData<data_type>*>(outbuff.get())->_data[i].begin(),
							   [this] (const data_type& x)
				{ return x * fft->normalizationFactor(); });
			}

			outputImpl->writeNextBuffer(outbuff);
		}

	private:
		Audio_p outbuff = nullptr;
};


