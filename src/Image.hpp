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


#pragma once

#include <opencv2/opencv.hpp>

#include <utils/NonCopyable.hpp>
#include <unordered_map>
#include <Pos.hpp>

namespace ocr {

    class Image
    {
        public:
            Image() = default;
            Image(std::string &&image_path);
            Image(cv::Mat &&matrix);
            ~Image() = default;

            Image(Image &&) = default;
            Image &operator=(Image &&) = default;

            void writeImage(std::string &&dest_path);
            Image subImage(cv::Rect &&r);
            void setMatrix(std::string &&id, cv::Mat &&mat);
            cv::Mat &getCurrentMatrix();
            cv::Mat const &getCurrentMatrix() const;

        private:
            void load(std::string &&image_path);

            cv::Mat m_currentMatrix;
            std::unordered_map<std::string, cv::Mat> m_matrices;

            cv::Mat m_smooth;
            cv::Mat m_threshold;
            cv::Mat m_open_morf;
    };
} // namespace ocr
