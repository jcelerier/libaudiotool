#pragma once
#include "FileInput.h"
#include "../mathutils/math_util.h"
#include <sstream>
//#include "ffmpeg/ffmpeg_wrapper.h"

// For system:
// #include <unistd.h>


template <typename data_type>
/**
 * @brief The FileInput class
 *
 * Convertit un fichier à l'aide de FFMPEG
 */
class FFMPEGFileInput : public FileInput<data_type>
{
	public:
		FFMPEGFileInput(std::string filename, Parameters<data_type>& cfg):
			FileInput<data_type>(cfg)
		{
			readFileFromSystem(filename);
		}

		FFMPEGFileInput(std::string filename, InputCopy<data_type>* icp, Parameters<data_type>& cfg):
			FileInput<data_type>(icp, cfg)
		{
			readFileFromSystem(filename);
		}

		/*
		virtual void readFile(std::string &str)
		{
			int sr;
			int nchans;
			auto vec = decode(str, sr, nchans);
			this->conf.samplingRate = sr;

			this->v().resize(nchans);
			for(int i = 0; i < nchans; i++)
			{
				this->v(i).resize(vec[i].size());
				std::transform(vec[i].begin(),
							   vec[i].end(),
							   this->v(i).begin(),
							   MathUtil::ToDouble<int16_t>);
			}
		}
		*/


		virtual void readFileFromSystem(std::string& str)
		{
			std::stringstream cmd;
			cmd << "ffmpeg -loglevel warning -y -i \""
				<< str
				<< "\" \""
				<< str
				<< ".wav\"";

			system(cmd.str().c_str());

			FileInput<data_type>::readFile(str.append(".wav"));
		}

};
