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

#include "NeuralNetworkClassifier.hpp"
#include "opencv2/opencv.hpp"

namespace ocr {

    NeuralNetworkClassifier::NeuralNetworkClassifier(
            utils::Json::Map const &datas
            )
        //: m_nbIterations(utils::get_item<int>(datas, "nb_iterations")),
                //m_stopRate(utils::get_item<float>(datas, "stop_rate")),
                //m_backpropogationCoef(
                    //utils::get_item<float>(datas, "backpropogation_coef")
                    //)
                //m_nbOutputClasses(features_extractor->nb_features())
                //m_featureExtractor(features_extractor)
    {
        std::cout << "coucou" << std::endl;
        std::cout << utils::get_item<int>(datas, "nb_iterations") << std::endl;
    }

    char
    NeuralNetworkClassifier::classify(cv::Mat &&featuresMatrix) const
    {
        cv::Mat classificationResult(1, m_nbOutputClasses, CV_32F);
        m_neuralNetwork.predict(featuresMatrix, classificationResult);
        return get_classification(classificationResult);
    }

    cv::Mat
    NeuralNetworkClassifier::get_data_matrix(std::vector<Dataset::Data> const &datas) const
    {
        auto value = cv::Mat();
        int index = 0;
        for (auto const &i: datas) {
            auto sourceMat = std::get<1>(i);
            std::cout << sourceMat << std::endl;
            value.row(index++) = sourceMat;
        }
        return cv::Mat();
    }

    void
    NeuralNetworkClassifier::train(Dataset &&d)
    {
        auto training_set = get_data_matrix(d.get_datas());

        // TODO: put the list of all output values
        auto training_set_classifications = cv::Mat();

        CvANN_MLP nnetwork(m_layers, CvANN_MLP::SIGMOID_SYM, 0.6, 1);

        CvANN_MLP_TrainParams params(
                cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS,
                               m_nbIterations,
                               m_stopRate),
                CvANN_MLP_TrainParams::BACKPROP,
                m_backpropogationCoef,
                m_backpropogationCoef
                );
        int iterations = nnetwork.train(training_set,
                                        training_set_classifications,
                                        cv::Mat(),
                                        cv::Mat(),
                                        params
                );

        std::cout << "\titerations\t" << iterations << std::endl;
    }

    void
    NeuralNetworkClassifier::serialize(std::string &&dest_path) const
    {
        CvFileStorage* storage = cvOpenFileStorage(dest_path.c_str(),
                                                   0,
                                                   CV_STORAGE_WRITE);
        m_neuralNetwork.write(storage,"DigitOCR");
        cvReleaseFileStorage(&storage);
    }

    char const *
    NeuralNetworkClassifier::name() const
    {
        return "NeuralNetworkClassifier";
    }

    char
    NeuralNetworkClassifier::get_classification(cv::Mat const &classificationResult) const
    {
        int maxIndex = 0;
        float value=0.0f;
        float maxValue = classificationResult.at<float>(0, 0);
        for (int index = 1; index < m_nbOutputClasses; ++index)
        {
            classificationResult.at<float>(0, index);
            if (value > maxValue)
            {
                maxValue = value;
                maxIndex = index;
            }
        }
        return values[maxIndex];
    }

} /* namespace ocr */

extern "C"
ocr::NeuralNetworkClassifier *
constructor(utils::Json::Map const &m)
{
    return new ocr::NeuralNetworkClassifier(m);
}
