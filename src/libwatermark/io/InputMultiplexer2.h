#pragma once
#include <iostream>
#include <memory>

#include "InputManagerBase.h"

template <typename data_type>
/**
 * @brief The InputManagerBase class
 *
 * Classe de base pour les entrées.
 */
class InputMultiplexer : public InputManagerBase<data_type>
{
	public:
		template<typename... I>
		InputMultiplexer(Parameters<data_type>& cfg, I&&... inputs):
			InputManagerBase<data_type>(cfg)
		{
			initInputs(inputs...);
		}

		InputMultiplexer(const InputMultiplexer<data_type>& orig) = delete;

		virtual ~InputMultiplexer() = default;

		virtual Audio_p getNextBuffer() override
		{
			auto out = new MultiData;
			for(auto i = 0U; i < _inputs.size(); ++i)
			{
				out->_data.push_back(_inputs[i]->getNextBuffer());
				if(!out->_data[i])
				{
					delete out;
					return Audio_p(nullptr);
				}
			}

			return Audio_p(out);
		}

	private:
		void initInputs(){} // Fin de récurrence

		template<typename Input, typename... I>
		void initInputs(Input&& i, I&&... args)
		{
			_inputs.push_back(i);
			initInputs(args...);
		}

		std::vector<Input_p> _inputs{};
};

