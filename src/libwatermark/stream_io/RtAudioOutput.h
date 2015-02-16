#pragma once


#include <RtAudio.h>
#include "StreamingOutputInterface.h"
#include "mathutils/math_util.h"
#include <functional>

int generate(void *outputBuffer, void *inputBuffer,
             unsigned int nFrames,
             double streamTime,
             RtAudioStreamStatus status,
             void *userData);

template<typename data_type>
class RtAudioOutput : public StreamingOutputInterface<data_type>
{
    friend int generate(void *outputBuffer, void *inputBuffer,
                        unsigned int nFrames,
                        double streamTime,
                        RtAudioStreamStatus status,
                        void *userData);
public:
    RtAudioOutput(Parameters<data_type>& cfg):
        StreamingOutputInterface<data_type>(cfg)
    {
        parameters.deviceId = audio.getDefaultOutputDevice();
        parameters.nChannels = 2;
        parameters.firstChannel = 0;
    }

    ~RtAudioOutput()
    {
        stopStream();
    }

    virtual void startStream(typename StreamingOutputInterface<data_type>::output_handler handle) override
    {
        using namespace std;
        using namespace placeholders;
        _handler = handle;

        try {
            unsigned int sampleRate = this->conf.samplingRate;
            unsigned int bufferFrames = this->conf.bufferSize;
            audio.openStream( &parameters, nullptr, RTAUDIO_FLOAT32,
                              sampleRate, &bufferFrames, &generate, this);
            qDebug() << "Stream opened";
            audio.startStream();
            qDebug() << "Stream started";

        }
        catch (RtAudioError &error) {
            error.printMessage();
            stopStream();
            return;
        }

        isRunning = true;

        while(isRunning)
        {
            sleep(1);
        }
    }

    virtual void stopStream()
    {
        try {
            qDebug() << Q_FUNC_INFO;
            // Stop and close the stream
            if(audio.isStreamRunning())
                audio.stopStream();

            if (audio.isStreamOpen())
                audio.closeStream();
            isRunning = false;
        }
        catch (RtAudioError &error) {
            error.printMessage();
        }
    }

private:

    typename StreamingOutputInterface<data_type>::output_handler _handler;

    RtAudio audio{RtAudio::LINUX_ALSA};
    RtAudio::StreamParameters parameters;
    bool isRunning{};
};


int generate(void *outputBuffer,
             void *inputBuffer,
             unsigned int nFrames,
             double streamTime,
             RtAudioStreamStatus status,
             void *userData)
{
    auto obj = static_cast<RtAudioOutput<float>*>(userData);
    if(nFrames < obj->conf.bufferSize)
    {
        std::cerr << "[WARNING] Buffer underrun.";
    }

    return obj->_handler(outputBuffer);
}
