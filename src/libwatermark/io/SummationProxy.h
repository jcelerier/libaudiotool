#pragma once

#pragma once


#include "InputManagerBase.h"

template <typename data_type>
class SummationProxy : public InputManagerBase<data_type>
{
		using InputManagerBase<data_type>::channels;
		using InputManagerBase<data_type>::frames;

	private:
		Input_p inputImpl = nullptr;

	public:
		SummationProxy(Input_p input):
			InputManagerBase<data_type>(nullptr,
										static_cast<InputManagerBase<data_type>*>(input.get())->conf),
			inputImpl{input}
		{
		}

		SummationProxy(InputManagerBase<data_type>* input):
			InputManagerBase<data_type>(nullptr,
										input->conf),
			inputImpl{input}
		{
		}

		virtual ~SummationProxy() = default;

		virtual Audio_p getNextBuffer() final override
		{
			Audio_p buf = inputImpl->getNextBuffer();

			auto numTracks = getMulti(buf).size();
			auto numChannels = getAudio<data_type>(getMulti(buf)[0]).size();
			auto bufferSize = getAudio<data_type>(getMulti(buf)[0])[0].size();

			if(firstRun)
			{
				firstRun = false;
				buffer.reset(new CData<data_type>{numChannels, bufferSize});
			}

			for(auto k = 0U; k < bufferSize; ++k)
			{
				for(auto j = 0U; j < numChannels; j++)
				{
					data_type sample{};
					for(auto i = 0U; i < numTracks; i++)
						sample += getAudio<data_type>(getMulti(buf)[i])[j][k];

					getAudio<data_type>(buffer)[j][k] = sample / numTracks;
				}
			}

			return buffer;
		}


	private:
		Audio_p buffer{nullptr};
		bool firstRun = true;
};
