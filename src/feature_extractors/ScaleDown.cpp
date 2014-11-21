// Simple OCR program
// Copyright (C) 2014 vhb
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <ScaleDown.hpp>

namespace ocr {

	ScaleDown::ScaleDown() {}

	ScaleDown::~ScaleDown() {}

	cv::Mat extract(Image const &img, int index) const {
		cv::Mat const & data = img.getSubMatrix(index);
		extract(data);
	}

	cv::Mat extract(cv::Mat const &m) const {
		cv::Mat dest;
		cv::Size size(width, height);
		cv::resize(m, dest, size);
	}

	int nb_features() const {
		return width * height;
	}

	char const *
	MomentExtractor::name() const
	{
		return "ScaleDown";
	}

}

extern "C"
ocr::ScaleDown *constructor()
{
	return new ocr::ScaleDown();
}