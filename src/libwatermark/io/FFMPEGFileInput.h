#pragma once
#include <unistd.h>
#include "FileInput.h"
#include "../mathutils/math_util.h"
#include <sstream>
#include "ffmpeg/ffmpeg_wrapper.h"

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
			readFile(filename);
		}

		FFMPEGFileInput(std::string filename, InputCopy<data_type>* icp, Parameters<data_type>& cfg):
			FileInput<data_type>(icp, cfg)
		{
			readFile(filename);
		}

		virtual void readFile(std::string &str) override
		{
			this->v().resize(2);
			auto vec = decode(str);
			this->v(0).resize(vec[0].size());
			this->v(1).resize(vec[1].size());

			for(int i = 0; i < 2; i++)
			{
				std::transform(vec[i].begin(),
							   vec[i].end(),
							   this->v(i).begin(),
							   MathUtil::ToDouble<int16_t>);
			}
		}

		virtual void readFileFromSystem(std::string& str)
		{
			std::stringstream cmd;
			cmd << "ffmpeg -y -i \""
				<< str
				<< "\" \""
				<< str
				<< ".wav\"";

			system(cmd.str().c_str());

			FileInput<data_type>::readFile(str.append(".wav"));
		}
};
