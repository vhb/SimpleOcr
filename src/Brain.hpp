#pragma once

#include <vector>
#include <memory>
#include <utils/json/json.hpp>

#include <ModuleManager.hpp>
#include <PreprocessorManager.hpp>
#include <ISegmenter.hpp>
#include <IClassifier.hpp>
#include <IPreprocessor.hpp>
#include <IFeatureExtractor.hpp>

#include <utils/json/json.hpp>
#include <utils/lib_loader.hpp>

namespace ocr {

    class Brain {

        public:
            Brain(std::string &&json_path);
            ~Brain() noexcept;

            bool train();
            std::vector<std::string> &&apply() const;

        private:


            PreprocessorManager                  m_preprocessorManager;
            std::shared_ptr<ISegmenter>          m_segmenter;
            std::shared_ptr<IFeatureExtractor>   m_featureExtractor;
            std::shared_ptr<IClassifier>         m_classifier;

            utils::Json                         m_json;
            ModuleManager                       m_moduleManager;
    };
}
