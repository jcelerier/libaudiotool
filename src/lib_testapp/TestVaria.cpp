#include "Pipable.h"
#include <benchmark/Amplify.h>
#include <benchmark/Pan.h>



void TestVaria()
{
	const Parameters<double> conf;
	auto volume = std::make_shared<Amplify<double>>(conf);
	auto pan = std::make_shared<Pan<double>>(conf);
	auto pan2 = std::make_shared<Pan<double>>(conf);
	
	
	std::function<void(std::vector<double>&)> f = [] (std::vector<double>& vec) { for(auto& elt : vec) elt *= 0.5; } ;
	std::function<double(const double&)> f2 = [] (const double& x) {return sin(x); } ;

	auto final = volume 
				 | pan 
				 | pan2 
				 | f 
				 | f2 
				 | ValFun(double, { return x + 4; }) // bof
				 | [] (const double & x) { return 1.0 / x; } ;
	
	Sequence<double> s(conf);
	s.initAlgos(f, f2);
	s.initAlgos([] (const double& x) {return sin(x); });
	
}
