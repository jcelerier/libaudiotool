#pragma once

#include "io/InputManagerBase.h"
#include "benchmark/Sequence.h"
#include "benchmark/BenchmarkFactory.h"

class Sequence_Wrapper : public Sequence<double>
{
        using Sequence<double>::Sequence;
};

template<typename A, typename B>
auto operator|(A&& a, B&& b) ->
    decltype(a->is_a_benchmark(),
             std::shared_ptr<Sequence<double>>())
{
    std::cerr << "operator|\n  a: "  << typeid(A).name()
              << "\n  b: " << typeid(B).name() << std::endl;
    return std::shared_ptr<Sequence<double>>(new Sequence<double>(a->conf, a, b));
}
/*
template<typename A, typename T>
auto operator|(A&& a, std::function<void(std::vector<T>&)> vect_func) -> std::shared_ptr<Sequence_Wrapper>
{
	return std::make_shared<Sequence_Wrapper>(a->conf, a, make_benchmark(vect_func));
}
*/
