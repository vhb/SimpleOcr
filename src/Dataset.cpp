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

#include <utils/json/Json.hpp>
#include <Dataset.hpp>
#include <unistd.h>
#include <algorithm>

namespace ocr {

    Dataset::Dataset(std::shared_ptr<IFeatureExtractor> const &features_extractor,
                     std::shared_ptr<PreprocessorManager> const &preprocessor_manager,
                     std::string const &json_path)
        : m_json_path(json_path),
          m_preprocessorManager(preprocessor_manager),
          m_featureExtractor(features_extractor)
    {
        auto json = utils::Json();
        auto datas = JSON_CAST(Map const, json.load(json_path));
        for (auto data : datas) {
            auto key = data.first;
            auto value = data.second;
            auto mat = cv::imread(JSON_CAST(String, value), 1);
            if (not mat.data) {
                throw std::runtime_error("Dataset::Dataset: Cannot oppen img file: " +
                                         JSON_CAST(String, value));
            }
            mat = m_preprocessorManager->apply(mat);
            auto features = m_featureExtractor->extract(mat);
            m_datas.push_back(Data(key, mat));
        }
        for (std::size_t i = 0; i < m_datas.size(); ++i) {
            get_output_for(i);
        }
    }

    Dataset::~Dataset() noexcept {}

    int
    Dataset::get_nb_output() const
    {
        return m_output.size();
    }

    int
    Dataset::get_output_for(std::size_t pos)
    {
        auto const &tmp = m_datas[pos];
        auto it = std::find_if(begin(m_output), end(m_output),
                [&] (std::string const &s){
            return s == std::get<0>(tmp);
        });
        if (it != end(m_output))
            return std::distance(begin(m_output), it);
        m_output.push_back(std::get<0>(tmp));
        return m_output.size() - 1;
    }

    std::vector<Dataset::Data> const &
    Dataset::get_datas() const
    {
        return m_datas;
    }

    std::string const &
    Dataset::get_json_path() const
    {
        return m_json_path;
    }

}
