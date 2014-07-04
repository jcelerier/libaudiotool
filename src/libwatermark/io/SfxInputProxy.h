#pragma once
#include <iostream>
#include <memory>

#include "InputManagerBase.h"

template <typename data_type>
class SfxInputProxy : public InputManagerBase<data_type>
{
	public:
		SfxInputProxy(Input_p input,
					  Benchmark_p algo):
			InputManagerBase<data_type>(static_cast<InputManagerBase<data_type>*>(input.get())->conf),
			_algorithm{algo},
			_input{input}
		{
		}

		SfxInputProxy(InputManagerBase<data_type>* input,
					  Benchmark_p algo):
			InputManagerBase<data_type>(input->conf),
			_algorithm{algo},
			_input{input}
		{
		}
		SfxInputProxy(InputManagerBase<data_type>* input,
					  BenchmarkBase<data_type>* algo):
			InputManagerBase<data_type>(input->conf),
			_algorithm{algo},
			_input{input}
		{
		}

		SfxInputProxy(const SfxInputProxy<data_type>& orig) = delete;
		virtual ~SfxInputProxy() = default;

		virtual Audio_p getNextBuffer() override
		{
			Audio_p buf = _input->getNextBuffer();

			if(buf) (*_algorithm)(buf);

			return  buf;
		}

	private:
		Benchmark_p _algorithm{};
		Input_p _input{};
};

