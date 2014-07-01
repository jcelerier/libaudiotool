#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#ifndef __arm__
#include <QtMultimedia/QAudioOutput>
#else
#include <QtMultimediaKit/QAudioOutput>
#endif
#define ENABLE_AUDIO

#include <QDebug>
#include <QBuffer>
class AudioManager : public QObject
{
		Q_OBJECT
	public:
		explicit AudioManager()
		{
			QList<QAudioDeviceInfo> ql = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
//			for(QAudioDeviceInfo dev : ql) qDebug() << dev.deviceName();
			data = new QByteArray;
			audioBuffer = new QBuffer(data);
			audioBuffer->open(QBuffer::ReadWrite);

			format.setSampleRate(44100);
			format.setChannelCount(2);
			format.setSampleSize(16);
			format.setCodec("audio/pcm");
			format.setByteOrder(QAudioFormat::LittleEndian);
			format.setSampleType(QAudioFormat::SignedInt);

			audioBuffer->seek(0);


			audioOut = new QAudioOutput(format, this);

		}

		void writeAudio(short* ext_buffer, unsigned int len)
		{
			data->append((const char*) ext_buffer, len * sizeof(short));

			if(audioOut->state() != QAudio::ActiveState)
				play();
			qDebug() << audioOut->periodSize();
		}

		void play()
		{
			audioOut->start(audioBuffer);
		}
		void stop()
		{
			if(audioOut->state() != QAudio::ActiveState)
			{
				audioOut->stop();
				audioBuffer->seek(0);
			}
		}

	public slots:
		void handleStateChanged(QAudio::State newState)
		{
			switch (newState) {
				case QAudio::IdleState:
					audioOut->start(audioBuffer);
					break;

				case QAudio::StoppedState:
					audioOut->start(audioBuffer);
					break;

				default:
					// ... other cases as appropriate
					break;
			}
		}

	private:
		QBuffer* audioBuffer{};
		QByteArray *data{};
		QAudioFormat format{};
		QAudioOutput *audioOut{};

};

#endif // AUDIOMANAGER_H
