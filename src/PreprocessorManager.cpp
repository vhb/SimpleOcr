#include <PreprocessorManager.hpp>
#include <utils/json/json.hpp>

namespace ocr {

    void
    PreprocessorManager::load_preprocessor(std::vector<utils::Json::Item> &&pl)
    {
        for (auto &i : pl) {
            auto &&item = JSON_CAST(Map, i);
            m_preprocessors.push_back(this->load_module<IPreprocessor>(std::move(item)));
        }
    }

} // namespace ocr
