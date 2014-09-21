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
			appendAlgorithm(algorithms...);
		}
		
		virtual void operator()(Audio_p& data) override
		{
			for(auto& effect : _effects) effect(data); // blehehee
		}
		
		std::vector<std::function<void(Audio_p&)>> _effects{};
		
		// Here be dragons ...
		void appendAlgorithm(){}
		
		// For chaining
		template<typename... A>
		void appendAlgorithm(std::shared_ptr<Sequence<data_type>> seq, A&&... args)
		{
			_effects.insert( _effects.end(), seq->_effects.begin(), seq->_effects.end() );
			
			appendAlgorithm(args...);
		}
		
		// For void(std::vector<data_type>&) stuff
		template<typename F, typename... A>
		auto appendAlgorithm(F&& f, A&&... args)
		-> decltype( f(std::declval<std::vector<data_type>&>()), void() )
		{
			_effects.push_back(std::bind(&vectorAlgorithmFactory<data_type>, std::placeholders::_1, f));
			
			appendAlgorithm(args...);
		}
		
		// For void(std::vector<std::vector<data_type>>&) stuff
		template<typename F, typename... A>
		auto appendAlgorithm(F&& f, A&&... args)
		-> decltype( f(std::declval<std::vector<std::vector<data_type>>&>()), void() )
		{
			_effects.push_back(std::bind(&channelAlgorithmFactory<data_type>, std::placeholders::_1, f));
			
			appendAlgorithm(args...);
		}
		
		
		// For data_type(const data_type&) stuff
		template<typename F, typename... A>
		auto appendAlgorithm(F&& f, A&&... args) -> typename
		std::enable_if
		<
			std::is_convertible
			<
				decltype(f(std::declval<const data_type&>())),
				data_type
			>::value,
			void
		>::type
		{
			_effects.push_back(std::bind(&valueAlgorithmFactory<data_type>, 
										 std::placeholders::_1, 
										 f));
			
			appendAlgorithm(args...);
		}
		
		// For Benchmark inherited stuff
		template<typename Algorithm, typename... A>
		auto appendAlgorithm(Algorithm&& a, A&&... args)
		-> typename
		std::enable_if
		<
			std::is_base_of
			<
				BenchmarkBase<data_type>,
				typename std::decay<Algorithm>::type::element_type
			>::value
		>::type
		{
			_effects.push_back(std::bind(&BenchmarkInterface::operator(),
										 a.get(), std::placeholders::_1));
			appendAlgorithm(args...);
		}
};
