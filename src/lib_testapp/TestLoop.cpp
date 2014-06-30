#include <io/FileInput.h>
#include <io/FileOutput.h>
#include <io/LoopInputProxy.h>

#include <manager/BenchmarkManager.h>
#include <manager/SummingManager.h>
#include <benchmark/Dummy.h>

#include <io/InputMultiplexer2.h>


void testMultiplex()
{
	Parameters<double> conf;
	auto input = Input_p(new InputMultiplexer<double>(conf,
							 Input_p(new LoopInputProxy<double>(Input_p(new FileInput<double>("input_mono.wav", conf)), conf)),
							 Input_p(new LoopInputProxy<double>(Input_p(new FileInput<double>("input_sfx_mono.wav", conf)), conf))));

	auto zeO = new FileOutput<double>(conf);
	auto output = Output_p(zeO);

	SummingManager<double> manager(std::move(input), std::move(output));

	manager.execute();

	zeO->writeFile("output_loop_sum.wav");
}

void testLoop()
{
	Parameters<double> conf;
	auto input = Input_p(new LoopInputProxy<double>(Input_p(new FileInput<double>("input_ramp.wav", conf)), conf));

	auto zeO = new FileOutput<double>(conf);
	auto output = Output_p(zeO);

	BenchmarkManager manager(std::move(input),
							 std::move(output),
							 Benchmark_p(new Dummy<double>(conf)));

	manager.execute();

	zeO->writeFile("output_loop.wav");
}

void TestLoop()
{
	//testLoop();
	testMultiplex();
}
