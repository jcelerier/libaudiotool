#pragma once
#include <unistd.h>
#include "FileInput.h"
#include "../mathutils/math_util.h"
#include <sstream>

template <typename data_type>
/**
 * @brief The FileInput class
 *
 * Lit un fichier à l'aide de libsndfile
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

		virtual void readFile(std::string& str) override
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
