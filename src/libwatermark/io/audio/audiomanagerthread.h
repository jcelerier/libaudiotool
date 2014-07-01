#pragma once
#include <QThread>
#include "audiomanager.h"

class AudioManagerThread : public QThread
{
		AudioManager* _manager;

	public slots:
		void writeAudio(short* ext_buffer, unsigned int len)
		{
			_manager->writeAudio(ext_buffer, len);
		}

	private slots:
		virtual void run()
		{
			_manager = new AudioManager;
			_manager->play();
			exec();
		}
};
