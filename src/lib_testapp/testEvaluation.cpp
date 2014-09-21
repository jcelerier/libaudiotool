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
#include "io/copystyle/InputFilter.h"
#include "io/copystyle/OutputFilter.h"
#include "evaluation/BitError.h"
#include "watermarkdata/SimpleWatermarkData.h"
#include "manager/WatermarkManager.h"
#include "manager/BenchmarkManager.h"
#include "watermark/LSBEncode.h"
#include "watermark/LSBDecode.h"
#include "watermark/RLSBEncode.h"
#include "watermark/RLSBDecode.h"
#include "watermark/SSWEncode.h"
#include "watermark/SSWDecode.h"
#include <string>
#include <algorithm>
#include <random>
#include "benchmark/Amplify.h"
#include "benchmark/Convolution.h"
#include "benchmark/FFTAmplify.h"
#include "benchmark/Dummy.h"
#include "mathutils/ssw_utils.h"


#include "Parameters.h"
#include "Data.h"
#include "watermarkdata/WatermarkData.h"
#include "watermark/WatermarkBase.h"

void TestEvaluation()
{
	BitError<double, Dummy<double>, short, LSBEncode<short>, LSBDecode<short>> lsbDummy;
	lsbDummy.execute("input_mono.wav");
	std::cout << "Dégradation : Dummy ====> False bits = " << lsbDummy.getError() << std::endl;

    std::cout << "******** Algorithme de watermark : LSB ********" << std::endl;
    BitError<double, Amplify<double>, short, LSBEncode<short>, LSBDecode<short>> lsbAmplify;
    lsbAmplify.execute("input_mono.wav");
    std::cout << "Dégradation : Amplify ====> False bits = " << lsbAmplify.getError() << std::endl;

    BitError<double, Convolution<double>, short, LSBEncode<short>, LSBDecode<short>> lsbConvolution;
    lsbConvolution.execute("input_mono.wav");
    std::cout << "Dégradation : Convolution ====> False bits = " << lsbConvolution.getError() << std::endl;

    BitError<double, FFTAmplify<double>, short, LSBEncode<short>, LSBDecode<short>> lsbFFTAmplify;
    lsbFFTAmplify.execute("input_mono.wav");
    std::cout << "Dégradation : FFTAmplify ====> False bits = " << lsbFFTAmplify.getError() << std::endl;

    std::cout << "******** Algorithme de watermark : RLSB ********" << std::endl;
    BitError<double, Amplify<double>, short, RLSBEncode<short>, RLSBDecode<short>> rlsbAmplify;
    rlsbAmplify.execute("input_mono.wav");
    std::cout << "Dégradation : Amplify ====> False bits = " << rlsbAmplify.getError() << std::endl;

    BitError<double, Convolution<double>, short, RLSBEncode<short>, RLSBDecode<short>> rlsbConvolution;
    rlsbConvolution.execute("input_mono.wav");
    std::cout << "Dégradation : Convolution ====> False bits = " << rlsbConvolution.getError() << std::endl;

    BitError<double, FFTAmplify<double>, short, RLSBEncode<short>, RLSBDecode<short>> rlsbFFTAmplify;
    rlsbFFTAmplify.execute("input_mono.wav");
    std::cout << "Dégradation : FFTAmplify ====> False bits = " << rlsbFFTAmplify.getError() << std::endl;

    std::cout << "******** Algorithme de watermark : SSW ********" << std::endl;
	// Données propres à SSW
	// Setup propre à SSW
	int size = 50;
	auto PNSequence = SSWUtil::generatePNSequence(size);

	BitError<double, Amplify<double>, double, SSWEncode<double>, SSWDecode<double>> sswAmplify;
	auto FrequencyRange = SSWUtil::generateFrequencyRange(size, sswAmplify.wparams);
	// Setup des algos
	SSWEncode<double>* e = static_cast<SSWEncode<double>*>(sswAmplify.encodeAlgo.get());
	SSWDecode<double>* d = static_cast<SSWDecode<double>*>(sswAmplify.decodeAlgo.get());
	e->setWatermarkAmp(10.0);
	e->setPNSequence(PNSequence);
	e->setFreqWinIndexes(FrequencyRange);
	d->setThreshold(0.5);
	d->setWatermarkAmp(10.0);
	d->setPNSequence(PNSequence);
	d->setFreqWinIndexes(FrequencyRange);

    sswAmplify.execute("input_mono.wav");
    std::cout << "Dégradation : Amplify ====> False bits = " << sswAmplify.getError() << std::endl;

	BitError<double, Convolution<double>, double, SSWEncode<double>, SSWDecode<double>> sswConvolution;
	e = static_cast<SSWEncode<double>*>(sswConvolution.encodeAlgo.get());
	d = static_cast<SSWDecode<double>*>(sswConvolution.decodeAlgo.get());
	e->setWatermarkAmp(10.0);
	e->setPNSequence(PNSequence);
	e->setFreqWinIndexes(FrequencyRange);
	d->setThreshold(0.5);
	d->setWatermarkAmp(10.0);
	d->setPNSequence(PNSequence);
	d->setFreqWinIndexes(FrequencyRange);

    sswConvolution.execute("input_mono.wav");
    std::cout << "Dégradation : Convolution ====> False bits = " << sswConvolution.getError() << std::endl;

	BitError<double, FFTAmplify<double>, double, SSWEncode<double>, SSWDecode<double>> sswFFTAmplify;
	e = static_cast<SSWEncode<double>*>(sswFFTAmplify.encodeAlgo.get());
	d = static_cast<SSWDecode<double>*>(sswFFTAmplify.decodeAlgo.get());
	e->setWatermarkAmp(10.0);
	e->setPNSequence(PNSequence);
	e->setFreqWinIndexes(FrequencyRange);
	d->setThreshold(0.5);
	d->setWatermarkAmp(10.0);
	d->setPNSequence(PNSequence);
	d->setFreqWinIndexes(FrequencyRange);

    sswFFTAmplify.execute("input_mono.wav");
    std::cout << "Dégradation : FFTAmplify ====> False bits = " << sswFFTAmplify.getError() << std::endl;

}
