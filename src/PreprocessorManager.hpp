#pragma once

#include <IPreprocessor.hpp>
#include <ModuleManager.hpp>
#include <utils/json/json.hpp>

namespace ocr {

    class PreprocessorManager : private ModuleManager {
        public:
            typedef std::vector<std::shared_ptr<IPreprocessor>> PreprocessorList;

            // In fact except a std::vector<std::string>
            void load_preprocessor(std::vector<utils::Json::Item> &&preprocessor_list);

            bool apply(Image &&img) const;
            PreprocessorList get_preprocessors() const;
            std::shared_ptr<IPreprocessor *> &&get_preprocessor(std::string &&name);

        private:
            PreprocessorList m_preprocessors;
    };
}
