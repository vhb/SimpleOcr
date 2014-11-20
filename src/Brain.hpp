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

#pragma once

#include <vector>
#include <memory>

#include <ModuleManager.hpp>
#include <PreprocessorManager.hpp>
#include <ISegmenter.hpp>
#include <IClassifier.hpp>
#include <IPreprocessor.hpp>
#include <IFeatureExtractor.hpp>

#include <utils/json/Json.hpp>
#include <utils/lib_loader.hpp>

namespace ocr {

    class Brain {

        public:
            Brain(std::string &&json_path, std::string &&dataset_path);
            ~Brain() ;

            void train();
            std::vector<std::string> apply(std::string const &image_path) const;
            void loadTrainData(std::string const &filePath);

        private:
            void update_json(utils::Json::Map &m);
            // The order is important here: the modulemanager destructor release
            //      The shared library used by the segment, etc..
            // This is a design flow, it may need fix, but it work for now
            ModuleManager                        m_moduleManager;
            std::shared_ptr<PreprocessorManager> m_preprocessorManager;
            std::shared_ptr<ISegmenter>          m_segmenter;
            std::shared_ptr<IFeatureExtractor>   m_featureExtractor;
            std::shared_ptr<IClassifier>         m_classifier;
            Dataset m_dataset;

            utils::Json                         m_json;
    };
}
