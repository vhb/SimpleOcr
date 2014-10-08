#include <Brain.hpp>
#include <utility>

//#include <

namespace ocr {

Brain::Brain(std::string &&json_path)
{
    auto datas = JSON_CAST(Map, m_json.load(std::move(json_path)));
    m_preprocessorManager.load_preprocessor(JSON_CAST(Vector, datas["preprocessor"]));
    m_segmenter = m_moduleManager.load_module<ISegmenter>(JSON_CAST(Map, datas["segmenter"]));
    m_featureExtractor = m_moduleManager.load_module<IFeatureExtractor>(JSON_CAST(Map, datas["segmenter"]));
    m_classifier = m_moduleManager.load_module<IClassifier>(JSON_CAST(Map, datas["classifier"]));
}

Brain::~Brain() noexcept
{
}

bool
Brain::train() {
    return false;
}

std::vector<std::string> &&
Brain::apply() const {
    return std::move(std::vector<std::string>());
}

}
