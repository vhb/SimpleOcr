#include <Brain.hpp>
#include <utility>

namespace ocr {

Brain::Brain(std::string &&json_path)
{
    using utils::Json::cast;
    auto datas = m_json.load(json_path);
    m_preprocessorManager.load(cast<utils::json::Vector>(m_json["preprocessor"]));
    m_segmenter = m_moduleManager.load<ISegmenter *>(cast<utils::json::Map>(m_json["segmenter"]));
    m_featureExtractor = m_moduleManager.load<IFeatureExtractor *>(cast<utils::json::Map>(m_json["segmenter"]));
    m_classifier = m_moduleManager.load<IClassifier *>(cast<utils::json::Map);
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
