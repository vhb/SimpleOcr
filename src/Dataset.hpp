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

#include <memory>
#include <tuple>

#include <opencv2/opencv.hpp>

#include <IFeatureExtractor.hpp>
#include <PreprocessorManager.hpp>

namespace ocr {

    // implement a range enumerable dataset
    class Dataset {
        public:
            typedef std::tuple<std::string, cv::Mat> Data;

            // Load the data described in `json_path`
            Dataset() = default;
            Dataset(std::shared_ptr<IFeatureExtractor> const &features_extractor,
                    std::shared_ptr<PreprocessorManager> const &preprocessor_manager,
                    std::string const &json_path);
            ~Dataset() noexcept;

            std::vector<Data> const &get_datas() const;
            std::string const &get_json_path() const;
            int get_nb_output() const;
            int get_output_for(std::size_t pos);
            std::string const &get_value(std::size_t pos) const;

        private:
            std::string m_json_path;
            std::shared_ptr<PreprocessorManager> m_preprocessorManager;
            std::shared_ptr<IFeatureExtractor> m_featureExtractor;
            std::vector<Data> m_datas;
            std::vector<std::string> m_output;
    };
}
