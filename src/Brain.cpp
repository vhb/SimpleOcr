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

#include <Brain.hpp>
#include <utility>

namespace ocr {

Brain::Brain(std::string &&json_path)
{
    auto datas = JSON_CAST(Map, m_json.load(std::move(json_path)));

    if (std::string(datas["preprocessors"].type().name()) == "v")
        throw std::runtime_error("No preprocessors in json");
    m_preprocessorManager.load_preprocessor(
            JSON_CAST(Vector, datas["preprocessors"])
            );

    if (std::string(datas["segmenter"].type().name()) == "v")
        throw std::runtime_error("No segmenter in json");
    m_segmenter = m_moduleManager.load_module<ISegmenter>(
            JSON_CAST(Map, datas["segmenter"])
            );

    // if (std::string(datas["feature_extractor"].type().name()) == "v")
        // throw std::runtime_error("No feature_extractor in json");
    // m_featureExtractor = m_moduleManager.load_module<IFeatureExtractor>(
            // JSON_CAST(Map, datas["feature_extractor"])
            // );

    // if (std::string(datas["classifier"].type().name()) == "v")
        // throw std::runtime_error("No classifier in json");

    // m_classifier = m_moduleManager.load_module<IClassifier>(
            // JSON_CAST(Map, datas["classifier"])
            // );
}

Brain::~Brain()
{
}

bool
Brain::train() {
#warning "TODO: Brain::train"
    return false;
}

std::vector<std::string> &&
Brain::apply(std::string &&imagePath) const {
    auto img = Image(std::move(imagePath));
    m_preprocessorManager.apply(img);
    m_segmenter->apply(std::move(img));
#warning "TODO: Brain::apply"
    return std::move(std::vector<std::string>());
}

}
