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
#include <stdexcept>

namespace ocr {

    //Image::Image(std::string &&image_path)
    //{
        //this->load(std::move(image_path));
    //}

    Image::Image(std::string const &image_path)
    {
        this->load(image_path);
    }

    Image::Image(cv::Mat &&matrix)
        : m_currentMatrix(matrix)
    {
        m_matrices["origin"] = matrix;
    }

    Image::~Image()
    {
    }

    Image
    Image::subImage(cv::Rect &&r)
    {
        cv::Mat sub_matrix = m_currentMatrix(r);
        m_currentMatrix.copyTo(sub_matrix);
        auto value = Image(std::move(sub_matrix));
        return value;
    }

    void
    Image::load(std::string const &img_path)
    {
        cv::Mat image = cv::imread(img_path, 1);
        if (image.empty()) {
            throw std::runtime_error("Image::load: Can't read image : " + img_path);
        }
        m_currentMatrix = image;
        m_matrices["origin"] = image;
    }

    void
    Image::writeImage(std::string const &dest_path)
    {
        for (auto const &i : m_matrices) {
            std::vector<int> compression_params;
            compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
            compression_params.push_back(9);
            auto tmp = dest_path + i.first + std::string(".png");
            std::cout << "writting image " << tmp << std::endl;
            cv::imwrite(tmp,
                        i.second,
                        compression_params);
        }
    }

    void
    Image::setMatrix(std::string const &matrixName, cv::Mat &&mat)
    {
        m_currentMatrix = mat;
        m_matrices[matrixName] = mat;
    }

    ssize_t
    Image::addSubMatrix(cv::Rect &&pos)
    {
        m_subMatrices.push_back(std::move(pos));
        return m_subMatrices.size() - 1;
    }

    cv::Mat
    Image::getSubMatrix(ssize_t id) const
    {
        if (size_t(id) > m_subMatrices.size())
            throw std::runtime_error("Invalid matrix id");
        cv::Mat sub_matrix = m_currentMatrix(m_subMatrices[id]);
        m_currentMatrix.copyTo(sub_matrix);
        return sub_matrix;
    }

    cv::Mat &
    Image::getCurrentMatrix()
    {
        return m_currentMatrix;
    }

    cv::Mat const &
    Image::getCurrentMatrix() const
    {
        return m_currentMatrix;
    }

} // namespace ocr
