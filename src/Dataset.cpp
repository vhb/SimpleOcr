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

#include <utils/json/json.hpp>
#include <Dataset.hpp>


namespace ocr {

    Dataset::Dataset(std::shared_ptr<IFeatureExtractor> const &features_extractor,
                     std::string const &json_path)
        : m_json_path(json_path),
          m_featureExtractor(features_extractor)
    {
        std::cout << "pourquoi" << std::endl;
        auto json = utils::Json();
        std::cout << "coucou" << std::endl;
        auto datas = JSON_CAST(Map, json.load(json_path));
        std::cout << "pourquoi" << std::endl;
        for (auto data : datas) {
            auto key = data.first;
            auto value = data.second;
            auto d = cv::imread(JSON_CAST(String, value));
            auto features = m_featureExtractor->extract(d);
            m_datas.push_back(Data(key, d));
            // ici je dois integrer la feature extraction
            // enfin, je pense.
            // donc en fait
        }
    }

    Dataset::~Dataset() noexcept {}

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
