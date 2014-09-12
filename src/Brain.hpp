#pragma once

#include <vector>
#include <memory>
#include <utils/json.hpp>

namespace ocr {

    class Brain {

        public:
            Brain(std::string &&json_path);
            ~Brain() noexcept;

            bool train();
            std::vector<std::string> &&apply() const;

        private:


            std::shared_ptr<PreprocessorManager> m_preprocessorManager;
            std::shared_ptr<ISegmenter>          m_segmenter;
            std::shared_ptr<IFeatureExtractor>   m_featureExtractor;
            std::shared_ptr<IClassifier>         m_classifier;

            utils::Json                         m_json;
            utils::LibLoader                    m_libLoader;
    };
}
