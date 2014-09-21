#include "Pipable.h"
#include <benchmark/Amplify.h>
#include <benchmark/Pan.h>
#include "manager/BenchmarkManager.h"
#include "io/inputs/FileInput.h"
#include "io/outputs/FileOutput.h"

void TestVaria()
{
    Parameters<double> conf;
    auto volume = std::make_shared<Amplify<double>>(conf);
	
    std::function<void(std::vector<double>&)> f = [] (std::vector<double>& vec) { for(auto& elt : vec) elt *= 4; } ;
    std::function<double(const double&)> f2 = [] (const double& x) {return sin(x); };

    class Smoothie
    {
    public:
        double operator()(double in) { z = (in * b) + (z * a); return z; }
    private:
        double a=0.99, b=1. - a, z{};
    } smooth;

    auto final = std::make_shared<Sequence<double>>(conf) // A predefined effect (Volume setting)
               | f // Another volume
               | [] (std::vector<double>& y) // Reverse each buffer
                  {
                      for(decltype(y.size()) i = 0; i < y.size() / 2; ++i)
                          std::swap(y[i], y[y.size() - i - 1]);
                  }
               | smooth | smooth | smooth | f // Bring some smoothing
               | [threshold=0.2] (const double& x) // And distort
                 {
                     if (x>threshold || x<-threshold)
                         return abs(abs(fmod(x - threshold, threshold*4)) - threshold*2) - threshold;
                     return x;
                 }
               | smooth // and smooth again
               | [] (std::vector<std::vector<double>>& y) // Copy first channel into each other
                 {
                    for(decltype(y.size())  i = 1; i < y.size(); ++i)
                    {
                        y[i] = y[0];
                    }
                 };

    auto input = new FileInput<double>("input_mono.wav", conf);
    auto output = new FileOutput<double>(conf);

    BenchmarkManager manager{Input_p(input),
                             Output_p(output),
                             final};

    manager.execute();

    output->writeFile(std::string("out_test_lol.wav").c_str());
	
}
