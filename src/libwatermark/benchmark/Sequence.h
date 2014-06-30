#pragma once
#include "BenchmarkBase.h"

template <typename data_type>
class Sequence : public BenchmarkBase<data_type>
{
		using BenchmarkBase<data_type>::conf;
		using size_type = typename Parameters<data_type>::size_type;


	public:
		template<typename... A>
		Sequence(const Parameters<data_type>& configuration, A&&... algorithms):
			BenchmarkBase<data_type>("Sequence", configuration)
		{
			initAlgos(algorithms...);
		}

		virtual void operator()(Audio_p& data) override
		{
			for(auto& effect : _effects)(*effect)(data); // blehehee
		}

	private:
		void initAlgos(){} // Fin de récurrence

		template<typename Algos, typename... A>
		void initAlgos(Algos&& a, A&&... args)
		{
			_effects.emplace_back(a);
			initAlgos(args...);
		}

		std::vector<Benchmark_p> _effects{};
};
