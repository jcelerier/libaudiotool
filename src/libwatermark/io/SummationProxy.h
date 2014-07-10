#pragma once

#pragma once


#include "InputManagerBase.h"

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

			// TODO Optimize
			for(auto j = 0U; j < numChannels; j++)
				for(auto k = 0U; k < bufferSize; k++)
					audio[j][k] = 0;

			for(auto k = 0U; k < bufferSize; ++k)
			{
				for(auto j = 0U; j < numChannels; j++)
				{
					for(auto i = 0U; i < numTracks; i++)
						audio[j][k] += getAudio<data_type>(getMulti(buf)[i])[j][k];
				}
			}

			// TODO Optimize
			for(auto j = 0U; j < numChannels; j++)
				for(auto k = 0U; k < bufferSize; k++)
					audio[j][k] /= numTracks;

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
