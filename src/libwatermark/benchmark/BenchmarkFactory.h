#pragma once
#include "mathutils/math_util.h"
#include <functional>

template<typename data_type>
void valueAlgorithmFactory(Audio_p& data,
                            std::function<data_type(const data_type&)> f)
{
    auto& channelsData = getAudio<data_type>(data);

    for(auto& sampleData : channelsData)
    {
        MathUtil::apply(sampleData, f);
    }
}

template<typename data_type>
void vectorAlgorithmFactory(Audio_p& data,
                             std::function<void(std::vector<data_type>&)> f)
{
    auto& channelsData = getAudio<data_type>(data);

    for(auto& sampleData : channelsData)
    {
        f(sampleData);
    }
}


template<typename data_type>
void channelAlgorithmFactory(Audio_p& data,
                             std::function<void(std::vector<std::vector<data_type>>&)> f)
{
    f(getAudio<data_type>(data));
}

