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
#include <Image.hpp>

namespace ocr {

Brain::Brain(std::string &&json_path, std::string &&dataset_path)
    : m_preprocessorManager(new PreprocessorManager())
{
    utils::Json::Map datas = JSON_CAST(Map const, m_json.load(std::move(json_path)));

    if (not datas["preprocessors"])
        throw std::runtime_error("No preprocessors in json");

    m_preprocessorManager->load_preprocessor(
            JSON_CAST(Vector const, datas["preprocessors"])
            );

    if (not datas["segmenter"])
        throw std::runtime_error("No segmenter in json");

    m_segmenter = m_moduleManager.load_module<ISegmenter>(
            (JSON_CAST(Map const, datas["segmenter"]))
            );

    if (not datas["feature_extractor"])
        throw std::runtime_error("No feature_extractor in json");
    m_featureExtractor = m_moduleManager.load_module<IFeatureExtractor>(
            JSON_CAST(Map const, datas["feature_extractor"])
            );

    if (not datas["classifier"])
        throw std::runtime_error("No classifier in json");

    auto tmp = datas["classifier"].get<utils::Json::Map>();
    JSON_CAST(Map, tmp["args"])["feature_extractor"] = m_featureExtractor;
    m_classifier = m_moduleManager.load_module<IClassifier>(tmp);
    m_dataset = Dataset(m_featureExtractor, m_preprocessorManager, dataset_path);

}

Brain::~Brain()
{
}

std::vector<std::string>
Brain::apply(std::string const &imagePath) const
{
    auto img = Image(std::move(imagePath));
    m_preprocessorManager->apply(img);
    std::cout << "Segmentation" << std::endl;
    std::cout << "\tApply " << m_segmenter->name() << std::endl;
    std::cout << "Looping over sub matrices" << std::endl;
    auto nb_subMatrix = m_segmenter->apply(std::move(img));
    std::cout << nb_subMatrix << std::endl;
    for (ssize_t i = 0; i < nb_subMatrix; ++i) {
        auto subMatrix = img.getSubMatrix(i);
        std::cout << "\t Feature extractor: " << m_featureExtractor->name()
                  << std::endl;
        auto features = m_featureExtractor->extract(std::move(img), i);
        std::cout << "\t Classifier: " << m_classifier->name() << std::endl;
        auto value = m_classifier->classify(std::move(features), m_dataset);
        std::cout << "output value: " << value << std::endl;
    }
    img.writeImage();
    return std::vector<std::string>();
}

void
Brain::loadTrainData(std::string const &filePath)
{
    m_classifier->unserialize(filePath);
}

void
Brain::train()
{
    std::cout << "Brain::train" << std::endl;
    //auto dataset = Dataset(m_featureExtractor,
                           //m_preprocessorManager,
                           //dataset_path);
    m_classifier->train(std::move(m_dataset));
}

void
Brain::serialize(std::string &&filePath)
{
    m_classifier->serialize(std::move(filePath));
}

void
Brain::update_json(utils::Json::Map &)
{
#warning "TODO: implement depencies injection"
}

}
