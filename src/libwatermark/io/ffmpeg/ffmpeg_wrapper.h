#pragma once

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
#include <vector>

template <typename IntegerType>
IntegerType bitsToInt(const unsigned char* bits, bool little_endian = true )
{
	IntegerType result = 0;
	if (little_endian)
		for (int n = sizeof( result ); n >= 0; n--)
			result = (result << 8) +bits[ n ];
	else
		for (unsigned n = 0; n < sizeof( result ); n++)
			result = (result << 8) +bits[ n ];
	return result;
}

void printAudioFrameInfo(const AVCodecContext* codecContext, const AVFrame* frame, std::vector<std::vector<int16_t>>& vec)
{
	if (codecContext->channels > AV_NUM_DATA_POINTERS && av_sample_fmt_is_planar(codecContext->sample_fmt))
	{
		// Copy from frame->extended_data. Doesn't need deinterlacing.
		std::cerr << "TODO (case > 8 channels)";
	}
	else if(av_sample_fmt_is_planar(codecContext->sample_fmt))
	{
		// Copy from frame->data. Doesn't need deinterlacing.
		//std::cerr << "Good Case : \n" << frame->linesize[0] << " " << frame->linesize[1] << std::endl ;

		for(int i = 0; i < frame->linesize[0] ; i += av_get_bytes_per_sample(codecContext->sample_fmt))
		{
			vec[0].push_back(bitsToInt<int16_t>(&frame->data[0][i]));
			vec[1].push_back(bitsToInt<int16_t>(&frame->data[1][i]));
		}
	}
	else
	{
		std::cerr << "TODO";
	}
}

std::vector<std::vector<int16_t>> decode(std::string& filename, int& sampleRate, int& channels)
{
	std::vector<std::vector<int16_t>> vec(2);
	// Initialize FFmpeg
	av_register_all();

	AVFrame* frame = av_frame_alloc();
	if (!frame)
	{
		std::cerr << "Error allocating the frame" << std::endl;
	}

	AVFormatContext* formatContext = NULL;
	if (avformat_open_input(&formatContext, filename.c_str(), NULL, NULL) != 0)
	{
		av_free(frame);
		std::cerr << "Error opening the file" << std::endl;
	}

	if (avformat_find_stream_info(formatContext, NULL) < 0)
	{
		av_free(frame);
		avformat_close_input(&formatContext);
		std::cerr << "Error finding the stream info" << std::endl;
	}

	// Find the audio stream
	AVCodec* cdc = nullptr;
	int streamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, &cdc, 0);
	if (streamIndex < 0)
	{
		av_free(frame);
		avformat_close_input(&formatContext);
		std::cerr << "Could not find any audio stream in the file" << std::endl;
	}

	AVStream* audioStream = formatContext->streams[streamIndex];
	AVCodecContext* codecContext = audioStream->codec;
	codecContext->codec = cdc;
	sampleRate = codecContext->sample_rate;
	channels = codecContext->channels;

	if (avcodec_open2(codecContext, codecContext->codec, NULL) != 0)
	{
		av_free(frame);
		avformat_close_input(&formatContext);
		std::cerr << "Couldn't open the context with the decoder" << std::endl;
	}

	AVPacket readingPacket;
	av_init_packet(&readingPacket);

	// Read the packets in a loop
	while (av_read_frame(formatContext, &readingPacket) == 0)
	{
		if (readingPacket.stream_index == audioStream->index)
		{
			AVPacket decodingPacket = readingPacket;

			// Audio packets can have multiple audio frames in a single packet
			while (decodingPacket.size > 0)
			{
				// Try to decode the packet into a frame
				// Some frames rely on multiple packets, so we have to make sure the frame is finished before
				// we can use it
				int gotFrame = 0;
				int result = avcodec_decode_audio4(codecContext, frame, &gotFrame, &decodingPacket);

				if (result >= 0 && gotFrame)
				{
					decodingPacket.size -= result;
					decodingPacket.data += result;

					// We now have a fully decoded audio frame
					printAudioFrameInfo(codecContext, frame, vec);
					// std::cerr << std::to_string(int16_t(frame->data[0][0])) << std::to_string(frame->data[0][frame->nb_samples - 1]) << std::endl;
				}
				else
				{
					decodingPacket.size = 0;
					decodingPacket.data = nullptr;
				}
			}
		}

		// You *must* call av_free_packet() after each call to av_read_frame() or else you'll leak memory
		av_free_packet(&readingPacket);
	}

	// Some codecs will cause frames to be buffered up in the decoding process. If the CODEC_CAP_DELAY flag
	// is set, there can be buffered up frames that need to be flushed, so we'll do that
	if (codecContext->codec->capabilities & CODEC_CAP_DELAY)
	{
		av_init_packet(&readingPacket);
		// Decode all the remaining frames in the buffer, until the end is reached
		int gotFrame = 0;
		while (avcodec_decode_audio4(codecContext, frame, &gotFrame, &readingPacket) >= 0 && gotFrame)
		{
			// We now have a fully decoded audio frame
			printAudioFrameInfo(codecContext, frame, vec);
			// std::cerr << frame->data[0][0] << frame->data[0][frame->nb_samples - 1];
		}
	}

	// Clean up!
	av_free(frame);
	avcodec_close(codecContext);
	avformat_close_input(&formatContext);

	return vec;
}
