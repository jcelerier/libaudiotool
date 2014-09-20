#pragma once
#include "BenchmarkBase.h"
#include "mathutils/math_util.h"
#include <functional>


template <typename data_type>
class CustomValueBenchmark : public BenchmarkBase<data_type>
{
	public:
		CustomValueBenchmark(std::function<data_type(const data_type&)> f):
			BenchmarkBase<data_type>("", Parameters<double>()),
			_f(f)
		{
			
		}
		
		virtual void operator()(Audio_p& data) override
		{
			auto& channelsData = getAudio<data_type>(data);

			for(auto& sampleData : channelsData)
			{
				MathUtil::apply(sampleData, _f);
			}
		}
		
		std::function<data_type(const data_type&)> _f;
};

template <typename data_type>
class CustomVectorBenchmark : public BenchmarkBase<data_type>
{
	public:
		CustomVectorBenchmark(std::function<void(std::vector<data_type>&)> f):
			BenchmarkBase<data_type>("", Parameters<double>()),
			_f(f)
		{
			
		}
		
		virtual void operator()(Audio_p& data) override
		{
			auto& channelsData = getAudio<data_type>(data);

			for(auto& sampleData : channelsData)
			{
				_f(sampleData);
			}
		}
		
		std::function<void(std::vector<data_type>&)> _f;
};
/*
template<typename T>
Benchmark_p make_benchmark(std::function<void(std::vector<T>&)> f)
{
	return Benchmark_p();//Benchmark_p(new CustomBenchmark<T>(f));
}
*/