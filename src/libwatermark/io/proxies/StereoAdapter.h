#pragma once
#include <iostream>
#include <memory>

#include <io/inputs/InputManagerBase.h>
#include <Data.h>

template <typename data_type>
class StereoAdapter : public InputManagerBase<data_type>
{
	public:
		StereoAdapter(Input_p input):
			InputManagerBase<data_type>(static_cast<InputManagerBase<data_type>*>(input.get())->conf),
			_inputImpl{input}
		{
		}

		StereoAdapter(InputManagerBase<data_type>* input):
			InputManagerBase<data_type>(input->conf),
			_inputImpl{input}
		{
		}

		StereoAdapter(const StereoAdapter<data_type>& orig) = delete;
		virtual ~StereoAdapter() = default;

		virtual Audio_p getNextBuffer() override
		{
			Audio_p buf = _inputImpl->getNextBuffer();
			if(!buf) return buf;

			auto& tracks = getAudio<data_type>(buf);

			switch(tracks.size())
			{
				case 1:
					if(firstRun)
					{
						firstRun = false;
						_stereoBuffer.reset(new CData<data_type>{2,
																 this->conf.bufferSize});
					}

					std::copy(tracks[0].begin(),
							  tracks[0].end(),
							  getAudio<data_type>(_stereoBuffer)[0].begin());
					std::copy(tracks[0].begin(),
							  tracks[0].end(),
							  getAudio<data_type>(_stereoBuffer)[1].begin());


					// 5. On retourne
					return _stereoBuffer;

				case 2:
					return buf;

				default:
					std::cerr << "Only mono and stereo" << std::endl;
					return buf;
			}
		}

		virtual void reset() override
		{
			static_cast<InputManagerBase<data_type>*>(_inputImpl.get())->reset();
		}

	private:
		Audio_p _stereoBuffer{};
		bool firstRun{true};
		Input_p _inputImpl{};
};

