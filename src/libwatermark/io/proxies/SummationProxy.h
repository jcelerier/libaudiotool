#pragma once

#include <io/inputs/InputManagerBase.h>
#include <mathutils/math_util.h>

template <typename data_type>
class SummationProxy : public InputManagerBase<data_type>
{
		using InputManagerBase<data_type>::channels;
		using InputManagerBase<data_type>::frames;

	private:
		Input_p _inputImpl = nullptr;

	public:
		SummationProxy(Input_p input):
			InputManagerBase<data_type>(nullptr,
										static_cast<InputManagerBase<data_type>*>(input.get())->conf),
			_inputImpl{input}
		{
		}

		SummationProxy(InputManagerBase<data_type>* input):
			InputManagerBase<data_type>(nullptr,
										input->conf),
			_inputImpl{input}
		{
		}

		virtual ~SummationProxy() = default;

		virtual Audio_p getNextBuffer() final override
		{
			Audio_p buf = _inputImpl->getNextBuffer();

			auto numTracks = getMulti(buf).size();
			auto numChannels = getAudio<data_type>(getMulti(buf)[0]).size();
			auto bufferSize = getAudio<data_type>(getMulti(buf)[0])[0].size();

			if(firstRun)
			{
				firstRun = false;
				buffer.reset(new CData<data_type>{numChannels, bufferSize});
			}

			auto& audio = getAudio<data_type>(buffer);

			for(auto j = 0U; j < numChannels; ++j)
			{
				auto& summed_chan = audio[j];

				std::swap(summed_chan, getAudio<data_type>(getMulti(buf)[0])[j]);

				for(auto i = 1U; i < numTracks; ++i)
				{
					auto& chan = getAudio<data_type>(getMulti(buf)[i])[j];
					std::transform(std::begin(chan), std::end(chan), std::begin(summed_chan), std::begin(summed_chan),
					[] (data_type& x1, data_type& x2)
					{
						return x1 + x2;
					});
				}

				MathUtil::apply(summed_chan, [&] (data_type& elt) { return elt / numTracks; });
			}

			return buffer;
		}

		virtual void reset() override
		{
			_inputImpl->reset();
		}

	private:
		Audio_p buffer{nullptr};
		bool firstRun = true;
};
