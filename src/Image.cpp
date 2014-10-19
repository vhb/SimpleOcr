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


#include <Image.hpp>

namespace ocr {

    Image::Image(std::string &&image_path, bool lazy)
        : m_img_path(image_path)
    {
        if (not lazy)
            this->load();
    }

    Image::Image(std::string &&image_path, cv::Mat &&matrix)
        : m_img_path(image_path), m_matrix(matrix)
    {
    }

    Image &&
    Image::subImage(cv::Rect &&r)
    {
        cv::Mat sub_matrix = m_matrix(r);
        m_matrix.copyTo(sub_matrix);
        return std::move(Image(std::move(m_img_path), std::move(sub_matrix)));
    }

    void
    Image::load()
    {
        cv::Mat image = cv::imread(m_img_path, 1);
        if (image.empty()) {
            throw std::runtime_error("Canno't read image");
        }
        m_matrix = image;
    }

    void
    Image::writeImage(std::string &&dest_path)
    {
        std::vector<int> compression_params;
        compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
        compression_params.push_back(9);
        imwrite(dest_path, m_matrix, compression_params);
    }

} // namespace ocr
