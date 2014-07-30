#pragma once


#include "portaudiocpp/PortAudioCpp.hxx"
#include "StreamingOutputInterface.h"

using namespace portaudio;
template<typename data_type>
class PortaudioOutput : public StreamingOutputInterface<data_type>
{
	public:
		PortaudioOutput(Parameters<data_type>& cfg):
			StreamingOutputInterface<data_type>(cfg),
			stream(StreamParameters(DirectionSpecificStreamParameters::null(),
									DirectionSpecificStreamParameters(System::instance().defaultOutputDevice(),
																	  2,
																	  FLOAT32,
																	  false,
																	  System::instance().defaultOutputDevice().defaultLowOutputLatency(),
																	  NULL),
									this->conf.samplingRate,
									this->conf.bufferSize,
									paClipOff), *this, &PortaudioOutput::generate)
		{
		}

		~PortaudioOutput()
		{
			stream.close();
			System::instance().terminate();
		}

		virtual void startStream(typename StreamingOutputInterface<data_type>::output_handler handle) override
		{
			_handler = handle;

			stream.start();
			isRunning = true;

			while(isRunning)
			{
				System::instance().sleep(200);
			}

			if(!stream.isStopped())
			{
				stopStream();
			}
		}

		virtual void stopStream()
		{
			isRunning = false;

//			try
			{
				stream.stop();
			}
/*			catch(PaException& e)
			{
				std::cerr << "Error while stopping: " << e.what();
				stream.close();
			}
*/		}

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
		AutoSystem sys;
		MemFunCallbackStream<PortaudioOutput> stream;
};
