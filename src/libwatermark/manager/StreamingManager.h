#pragma once
#include <memory>
#include "ManagerBase.h"
#include "../Data.h"
#include "../stream_io/StreamingOutputInterface.h"

/**
 * @brief Main class.
 *
 */
template<typename data_type>
class StreamingManager: public ManagerBase
{
	public:
		StreamingManager(Input_p i,
						 std::shared_ptr<StreamingOutputInterface<data_type>> o,
						 std::function<void ()> h,
						 Parameters<data_type> cfg):
			ManagerBase(i, o),
			conf(cfg),
			timeHandle(h)
		{
		}

		int generate(void* outputBuffer )
		{
			float **out = static_cast<float **>(outputBuffer);

			Audio_p buffer = m_input->getNextBuffer();
			if(!buffer)
			{
				static_cast<StreamingOutputInterface<data_type>*>(output().get())->stopStream();
				return 1;
			}
			auto audio = getAudio<data_type>(buffer); // Should be stereo.

			std::copy(audio[0].begin(),
					  audio[0].end(),
					  out[0]);
			std::copy(audio[1].begin(),
					  audio[1].end(),
					  out[1]);

			timeHandle();

			return 0;
		}

		virtual void execute() override
		{
			static_cast<StreamingOutputInterface<data_type>*>(output().get())->startStream(std::bind(&StreamingManager::generate, this, std::placeholders::_1));
		}

		void stop()
		{
			// Remettre les positions à 0;

			// Stop
			static_cast<StreamingOutputInterface<data_type>*>(output().get())->stopStream();
		}

	private:
		Parameters<data_type>& conf;
		std::function<void ()> timeHandle;
};
