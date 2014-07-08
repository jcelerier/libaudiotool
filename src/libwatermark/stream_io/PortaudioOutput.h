#pragma once


#include "portaudiocpp/PortAudioCpp.hxx"
#include "StreamingOutputInterface.h"

using namespace portaudio;
template<typename data_type>
class PortaudioOutput : public StreamingOutputInterface<data_type>
{
	public:
		using StreamingOutputInterface<data_type>::StreamingOutputInterface;
		virtual void startStream(typename StreamingOutputInterface<data_type>::output_handler handle) override
		{
			_handler = handle;
			AutoSystem autoSys;

			MemFunCallbackStream<PortaudioOutput> stream(
						StreamParameters(DirectionSpecificStreamParameters::null(),
										 DirectionSpecificStreamParameters(System::instance().defaultOutputDevice(),
																		   2,
																		   FLOAT32,
																		   false,
																		   System::instance().defaultOutputDevice().defaultLowOutputLatency(),
																		   NULL),
										 this->conf.samplingRate,
										 this->conf.bufferSize,
										 paClipOff), *this, &PortaudioOutput::generate);
			stream.start();
			isRunning = true;

			while(isRunning)
			{
				System::instance().sleep(200);
			}

			stream.stop();
			stream.close();

			System::instance().terminate();
		}

		virtual void stopStream()
		{
			isRunning = false;
		}

	private:
		int generate(const void *, void *outputBuffer, unsigned long framesPerBuffer,
					 const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags )
		{
			if(framesPerBuffer < this->conf.bufferSize)
			{
				std::cerr << "[WARNING] Buffer underrun.";
			}
			return _handler(outputBuffer);
		}

		bool isRunning{false};
		typename StreamingOutputInterface<data_type>::output_handler _handler;
};
