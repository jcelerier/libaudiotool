#pragma once
#include <functional>

#include <io/outputs/OutputManagerBase.h>

template <typename data_type>
class StreamingOutputInterface : public OutputManagerBase<data_type>
{
	public:
		using OutputManagerBase<data_type>::OutputManagerBase;
		using output_handler = std::function<int (void *)>;
		virtual void startStream(StreamingOutputInterface::output_handler handle) = 0;
		virtual void stopStream() = 0;
};
