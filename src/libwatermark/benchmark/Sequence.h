#pragma once
#include "BenchmarkBase.h"
#include "BenchmarkFactory.h"
#include <type_traits>
#define ValFun(type, _fun) std::function<type(const type&)>{[&] (const type& x) _fun}
#define VecFun(type, _fun) std::function<void(std::vector<type>&)>{[&] (std::vector<type>& x) _fun}

template <typename data_type>
class Sequence : public BenchmarkBase<data_type>
{
		using size_type = typename Parameters<data_type>::size_type;

	public:
		using BenchmarkBase<data_type>::conf;
		template<typename... A>
		Sequence(const Parameters<data_type>& configuration, 
				 A&&... algorithms):
			BenchmarkBase<data_type>("Sequence", configuration)
		{
			initAlgos(algorithms...);
		}
		template<typename... A>
		Sequence(const Parameters<data_type>& configuration, 
				 std::function<void(std::vector<data_type>&)>&& f, 
				 A&&... algorithms):
			BenchmarkBase<data_type>("Sequence", configuration)
		{
			initAlgos(f, algorithms...);
		}

		virtual void operator()(Audio_p& data) override
		{
			for(auto& effect : _effects)(*effect)(data); // blehehee
		}

	
		void initAlgos(){} // Fin de récurrence

		template<typename Algos, typename... A>
		auto initAlgos(Algos&& a, A&&... args)
		-> decltype( a->is_a_benchmark(), void() )
		{
			_effects.emplace_back(a);
			initAlgos(args...);
		}
		
		template<typename... A>
		void initAlgos(std::shared_ptr<Sequence>&& seq, A&&... args)
		{
			_effects.insert( _effects.end(), seq->_effects.begin(), seq->_effects.end() );
			
			initAlgos(args...);
		}
		
		template<typename F, typename... A>
		auto initAlgos(F&& f, A&&... args)
		-> decltype( f(std::declval<std::vector<data_type>&>()), void() )
		{
			_effects.emplace_back(Benchmark_p(new CustomVectorBenchmark<data_type>(f)));
			
			initAlgos(args...);
		}
		template<typename F, typename... A>
		auto initAlgos(F&& f, A&&... args)
		-> typename std::enable_if< std::is_convertible<decltype(f(std::declval<const data_type&>())), data_type>::value, void>::type
		{
			_effects.emplace_back(Benchmark_p(new CustomValueBenchmark<data_type>(f)));
			
			initAlgos(args...);
		}

		std::vector<Benchmark_p> _effects{};
};
