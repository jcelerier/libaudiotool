#include <io/FileInput.h>
#include <io/FileOutput.h>
#include <io/LoopInputProxy.h>

#include <manager/BenchmarkManager.h>
#include <manager/StreamingManager.h>
#include <benchmark/Dummy.h>

#include <io/InputMultiplexer2.h>
#include <io/SfxInputProxy.h>
#include <io/StereoAdapter.h>

#include <benchmark/Pan.h>
#include <benchmark/Amplify.h>
#include <benchmark/Sequence.h>

#include <io/audio/QtAudioOutput.h>
#include <io/SummationProxy.h>

#include <stream_io/PortaudioOutput.h>

void testMultiplex()
{
	Parameters<double> conf;


	auto stereo_loop_file1 = new StereoAdapter<double>(new LoopInputProxy<double>(new FileInput<double>("beat1.wav", conf)));
	auto stereo_loop_file2 = new StereoAdapter<double>(new LoopInputProxy<double>(new FileInput<double>("beat2.wav", conf)));

	auto pan1 = new Pan<double>(conf);
	auto pan2 = new Pan<double>(conf);
	pan1->setPan(-0.4);
	pan2->setPan(0.8);
	auto fxchannel1 = Benchmark_p(new Sequence<double>(conf,
													   new Amplify<double>(conf),
													   pan1));

	auto fxchannel2 = Benchmark_p(new Sequence<double>(conf,
													   new Amplify<double>(conf),
													   pan2));

	auto input = Input_p(new SummationProxy<double>(
					 new InputMultiplexer<double>(conf,
												  new SfxInputProxy<double>(stereo_loop_file1,
																			fxchannel1),
												  new SfxInputProxy<double>(stereo_loop_file2,
																			fxchannel2))));


	auto zeO = new PortaudioOutput<double>(conf);
	auto output = std::shared_ptr<PortaudioOutput<double>>(zeO);

	StreamingManager<double> manager(std::move(input), std::move(output), conf);

	manager.execute();

	//zeO->writeFile("output_loop_sum.wav");
}

void testLoop()
{
	Parameters<double> conf;
	auto input = Input_p(new LoopInputProxy<double>(Input_p(new FileInput<double>("input_ramp.wav", conf))));

	auto zeO = new FileOutput<double>(conf);
	auto output = Output_p(zeO);

	BenchmarkManager manager(std::move(input),
							 std::move(output),
							 Benchmark_p(new Dummy<double>(conf)));

	manager.execute();

	zeO->writeFile("output_loop.wav");
}


void testStream()
{
	Parameters<double> conf;

	auto input = Input_p(new FileInput<double>("beat1.wav", conf));
	auto zeO = new PortaudioOutput<double>(conf);
	auto output = std::shared_ptr<PortaudioOutput<double>>(zeO);

	StreamingManager<double> manager(std::move(input), std::move(output), conf);

	manager.execute();
}

void TestLoop()
{
	//testLoop();
	testMultiplex();
	//testStream();
}
