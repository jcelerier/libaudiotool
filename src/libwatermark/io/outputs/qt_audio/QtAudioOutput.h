#pragma once

#include "../OutputManagerBase.h"
#include "audiomanagerthread.h"
#include <mathutils/math_util.h>
#include <thread>
#include <unistd.h>
template <typename data_type>
class QtAudioOutput : public OutputManagerBase<data_type>
{
	public:
		QtAudioOutput(Parameters<data_type> cfg):
			OutputManagerBase<data_type>(cfg)
		{
			t.start();
			sleep(1);
		}

		virtual ~QtAudioOutput()
		{
			t.exit();
			t.wait();
		}

		AudioManagerThread t;
		virtual void writeNextBuffer(Audio_p& data) override
		{

			// 1. Réentrelacement
			auto base_vec = MathUtil::interleave(getAudio<data_type>(data));

			// 2. Copie
			if(typeid(data_type) == typeid(short))
			{
				t.writeAudio((short*)base_vec.data(), base_vec.size());
			}
			else
			{
				std::vector<short> vect(base_vec.size());
				std::transform(base_vec.begin(),
							   base_vec.end(),
							   vect.begin(),
							   MathUtil::FromDouble<short>);

				t.writeAudio(vect.data(), vect.size());
			}

		}

	private:
	//	AudioManager _manager{};
};
