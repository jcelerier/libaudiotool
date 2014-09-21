#include <manager/BenchmarkManager.h>
#include <manager/StreamingManager.h>
#include <benchmark/Dummy.h>

#include <io/proxies/InputMultiplexer2.h>
#include <io/proxies/SfxInputProxy.h>
#include <io/proxies/StereoAdapter.h>

#include <benchmark/Pan.h>
#include <benchmark/Amplify.h>
#include <benchmark/Sequence.h>

#include <io/outputs/qt_audio/QtAudioOutput.h>
#include <io/proxies/SummationProxy.h>
#include <io/proxies/LoopInputProxy.h>

#include <stream_io/PortaudioOutput.h>

#include <io/inputs/FileInput.h>
#include <io/outputs/FileOutput.h>
#include <io/inputs/SilenceInput.h>
#include <io/proxies/fftproxy/FFTInputProxy.h>
#include <io/proxies/fftproxy/FFTOutputProxy.h>

#include <io/inputs/BufferInput.h>
#include <io/outputs/BufferOutput.h>
#include <io/inputs/SilenceInput.h>
#include <io/outputs/DummyOutput.h>
#include "watermarkdata/SimpleWatermarkData.h"
#include "timeadapter/Every.h"

#include <io/proxies/mcltproxy/MCLTInputProxy.h>
#include <io/proxies/mcltproxy/MCLTOutputProxy.h>
#include "watermark/DummyWatermark.h"
#include "mathutils/ssw_utils.h"
#include "transform/FFTWManager.h"
#include "watermark/SSWEncode.h"
#include "watermark/SSWDecode.h"

#include "TestHeader.h"

#include "io/copystyle/InputSimple.h"
#include "io/copystyle/InputOLA.h"
#include "io/copystyle/InputFilter.h"
#include "io/copystyle/OutputSimple.h"
#include "io/copystyle/OutputOLA.h"
#include "io/copystyle/OutputFilter.h"

#include "benchmark/Dummy.h"
#include "benchmark/Amplify.h"

#include "manager/BenchmarkManager.h"

#include "transform/FFTWManager.h"
/*
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

    StreamingManager<double> manager(input, output, conf);

	manager.execute();

	//zeO->writeFile("output_loop_sum.wav");
}
*/

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

/*
void testStream()
{
	Parameters<double> conf;

	auto input = Input_p(new FileInput<double>("beat1.wav", conf));
	auto zeO = new PortaudioOutput<double>(conf);
	auto output = std::shared_ptr<PortaudioOutput<double>>(zeO);

	StreamingManager<double> manager(std::move(input), std::move(output), conf);

	manager.execute();
}
*/
void TestLoop()
{
	//testLoop();
	//testMultiplex();
    //testStream();
}
