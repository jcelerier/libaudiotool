#pragma once

#include "ManagerBase.h"
#include "../Data.h"

/**
 * @brief Main class.
 *
 */
template<typename data_type>
class SummingManager: public ManagerBase
{
	public:
		using ManagerBase::ManagerBase;

		virtual void execute() override
		{
			while(Audio_p buf = m_input->getNextBuffer())
			{
				auto numTracks = getMulti(buf).size();
				auto channels = getAudio<data_type>(getMulti(buf)[0]).size();
				auto bufferSize = getAudio<data_type>(getMulti(buf)[0])[0].size();

				if(firstRun)
				{
					firstRun = false;
					buffer.reset(new CData<data_type>{channels, bufferSize});
				}

				for(auto k = 0U; k < bufferSize; ++k)
				{
					for(auto j = 0U; j < channels; j++)
					{
						data_type sample{};
						for(auto i = 0U; i < numTracks; i++)
							sample += getAudio<data_type>(getMulti(buf)[i])[j][k];

						getAudio<data_type>(buffer)[j][k] = sample / numTracks;
					}
				}
				m_output->writeNextBuffer(buffer);
			}
		}


	private:
		Audio_p buffer{nullptr};
		bool firstRun = true;
};
