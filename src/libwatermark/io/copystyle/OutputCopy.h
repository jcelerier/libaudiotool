#pragma once

#include "CopyStyle.h"

template <typename data_type>
class OutputCopy: public CopyStyle<data_type>
{
	public:
		using CopyStyle<data_type>::CopyStyle;
		virtual ~OutputCopy() = default;

		virtual typename CopyStyle<data_type>::size_type lastIncrement() override
		{
			return this->frameIncrement();
		}
};

template <typename T>
using OutputCopy_p = std::shared_ptr<OutputCopy<T>>;
