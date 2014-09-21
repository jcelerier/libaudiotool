#pragma once
#include <iostream>
#include <memory>

#include <io/IOManagerBase.h>
#include <io/copystyle/InputSimple.h>

template <typename data_type>
/**
 * @brief The InputManagerBase class
 *
 * Classe de base pour les entrées.
 */
class InputManagerBase : public IOManagerBase<data_type>, public InputManagerInterface
{
	public:
		using IOManagerBase<data_type>::pos;
		using IOManagerBase<data_type>::channels;
		using IOManagerBase<data_type>::frames;

		using IOManagerBase<data_type>::reset;


		using IOManagerBase<data_type>::IOManagerBase;

		InputManagerBase(InputCopy<data_type>* copy, Parameters<data_type>& cfg):
			IOManagerBase<data_type>(cfg),
			copyHandler(copy)
		{
		}

		InputManagerBase(const InputManagerBase<data_type>& orig) = delete;

		virtual ~InputManagerBase() {}

		virtual Audio_p getNextBuffer() override
		{
			if(pos() == 0 && firstRun) // Premier buffer
			{
				firstRun = false;
				buffer.reset(new CData<data_type>(channels(), this->conf.bufferSize));
			}

			if(pos() < frames())
			{
				// Remplissage pour chaque canal
				for(auto i = 0U; i < channels(); ++i)
				{
					copyHandler->copy(this->v()[i].begin(),
									  static_cast<CData<data_type>*>(buffer.get())->_data[i].begin(),
									  pos(),
									  frames());
				}

				pos() += copyHandler->lastIncrement();
				return buffer;
			}

			return Audio_p(nullptr);
		}

		virtual void reset()
		{
			pos() = 0;
		}

		InputCopy_p<data_type> copyHandler{InputCopy_p<data_type>(new InputSimple<data_type>(this->conf))};
		Audio_p buffer{nullptr};

		bool firstRun = true;
};

