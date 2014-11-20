// Simple ocr
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

#include <memory>
#include <Dataset.hpp>
#include <IClassifier.hpp>
#include <IFeatureExtractor.hpp>
#include <utils/json/Json.hpp>

namespace ocr {

    class NeuralNetworkClassifier
        : public IClassifier
    {
        public:
            NeuralNetworkClassifier(utils::Json::Map const &datas);
            virtual ~NeuralNetworkClassifier() noexcept {}

            virtual char classify(cv::Mat &&features,
                                  Dataset const &dataset) const;
            virtual void train(Dataset &&);
            virtual void serialize(std::string &&dest_path) const;
            virtual char const *name() const;
            virtual void unserialize(std::string const &);

        private:
            char get_classification(cv::Mat const &classification_matrix) const;
            cv::Mat get_classification_matrix(cv::Mat const &training_set,
                                              Dataset &&);
            cv::Mat get_data_matrix(std::vector<Dataset::Data> const &datas) const;

            cv::Mat m_layers;
            cv::Ptr<cv::ml::ANN_MLP> m_neuralNetwork;
            double m_nbIterations;
            double m_stopRate;
            double m_backpropogationCoef;
            int m_nbFeatures;
            int m_nbOutputClasses;
            std::shared_ptr<IFeatureExtractor> m_featureExtractor;
            std::vector<char> values;
            cv::Mat m_training_set;
            cv::Mat m_training_set_classifications;
    };

} // namespace ocr
