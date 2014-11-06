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

    if (!datas["preprocessors"])
        throw std::runtime_error("No preprocessors in json");

    m_preprocessorManager.load_preprocessor(
            std::move(JSON_CAST(Vector, datas["preprocessors"]))
            );

    if (!datas["segmenter"])
        throw std::runtime_error("No segmenter in json");
    m_segmenter = m_moduleManager.load_module<ISegmenter>(
            std::move(JSON_CAST(Map, datas["segmenter"]))
            );

    if (!datas["feature_extractor"])
        throw std::runtime_error("No feature_extractor in json");
    m_featureExtractor = m_moduleManager.load_module<IFeatureExtractor>(
            std::move(JSON_CAST(Map, datas["feature_extractor"]))
            );

    if (!datas["classifier"])
        throw std::runtime_error("No classifier in json");
    m_classifier = m_moduleManager.load_module<IClassifier>(
            std::move(JSON_CAST(Map, datas["classifier"]))
            );
}

Brain::~Brain()
{
}

std::vector<std::string>
Brain::apply(std::string &&imagePath) const
{
    auto img = Image(std::move(imagePath));
    m_preprocessorManager.apply(img);
    std::cout << "Segmentation" << std::endl;
    std::cout << "\tApply " << m_segmenter->name() << std::endl;
    std::cout << "Looping over sub matrices" << std::endl;
    auto nb_subMatrix = m_segmenter->apply(std::move(img));
    /*for (ssize_t i = 0; i < nb_subMatrix; ++i) {*/
        /*auto subMatrix = img.getSubMatrix(i);*/
        /*std::cout << "\t Feature extractor: " << m_featureExtractor->name()*/
                  /*<< std::endl;*/
        /*auto features = m_featureExtractor->extract(std::move(img), i);*/
        /*std::cout << "\t Classifier: " << m_classifier->name() << std::endl;*/
        /*auto value = m_classifier->classify(std::move(features));*/
        /*std::cout << "\t\tvalue: " << value << std::endl;*/
        /*[>break;<]*/
    /*}*/
    img.writeImage();
    return std::vector<std::string>();
}

void
Brain::train(std::string &&dataset_path)
{

}

void
Brain::update_json(utils::Json::Map &m)
{
    /*using namespace utils;*/
    /*Json::Item &it = *m.find("args");*/
    /*[>Json::Item it2 = JSON_CAST(Map, *m.find("args"));<]*/
    /*utils::Json::Map &tmp = JSON_CAST(Map, *m.find("args"));*/
    /*for (auto &item : tmp) {*/
        /*if (item.first == "__feature_exctractor__")*/
            /*item.second = Json::Item(m_featureExtractor);*/
        /*else if (item.first == "__segmenter__")*/
            /*item.second = m_segmenter;*/
        /*else if (item.first == "__preprocessorManager__")*/
            /*item.second = m_preprocessorManager;*/
        /*else if (item.first == "__classifier__")*/
            /*item.second = m_classifier;*/
    /*}*/
}

}
