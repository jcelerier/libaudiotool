#pragma once

#include "ManagerBase.h"
#include "../Data.h"

#include "portaudiocpp/PortAudioCpp.hxx"


const int NUM_SECONDS = 5;
const double SAMPLE_RATE = 44100.0;
const int FRAMES_PER_BUFFER = 1024;
const int TABLE_SIZE = 200;
/**
 * @brief Main class.
 *
 */
template<typename data_type>
class SummingManager: public ManagerBase
{
	public:
		SummingManager(Input_p i,
					   Output_p o):
			ManagerBase(i, o)
		{
			std::cout << "Setting up PortAudio..." << std::endl;

			// Set up the System:
			portaudio::AutoSystem autoSys;
			portaudio::System &sys = portaudio::System::instance();

			// Set up the parameters required to open a (Callback)Stream:
			portaudio::DirectionSpecificStreamParameters outParams(sys.defaultOutputDevice(),
																   2,
																   portaudio::FLOAT32,
																   false,
																   sys.defaultOutputDevice().defaultLowOutputLatency(),
																   NULL);

			portaudio::StreamParameters params(portaudio::DirectionSpecificStreamParameters::null(),
											   outParams,
											   SAMPLE_RATE,
											   FRAMES_PER_BUFFER,
											   paClipOff);

			std::cout << "Opening stereo output stream..." << std::endl;

			// Create (and open) a new Stream, using the SineGenerator::generate function as a callback:
			portaudio::MemFunCallbackStream<SummingManager> stream(params, *this, &SummingManager::generate);

			std::cout << "Starting playback for " << NUM_SECONDS << " seconds." << std::endl;

			// Start the Stream (audio playback starts):
			stream.start();

			// Wait for 5 seconds:
			while(1) sys.sleep(NUM_SECONDS * 1000);

			std::cout << "Closing stream..." <<std::endl;

			// Stop the Stream (not strictly needed as termintating the System will also stop all open Streams):
			stream.stop();

			// Close the Stream (not strictly needed as terminating the System will also close all open Streams):
			stream.close();

			// Terminate the System (not strictly needed as the AutoSystem will also take care of this when it
			// goes out of scope):
			sys.terminate();
		}

		int generate(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
			const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags)
		{
			float **out = static_cast<float **>(outputBuffer);

			Audio_p buf = m_input->getNextBuffer();
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

					out[j][k] =  sample / numTracks;
				}
			}

			return paContinue;
		}




		virtual void execute() override
		{
			return;

			while(1)
			{
				Audio_p buf = m_input->getNextBuffer();
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

				//m_output->writeNextBuffer(buffer);
			}
		}


	private:
		Audio_p buffer{nullptr};
		bool firstRun = true;
};
