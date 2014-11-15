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
        try :   m_nbIterations(utils::get_item<int>(datas, "nb_iterations")),
                m_stopRate(utils::get_item<float>(datas, "stop_rate")),
                m_backpropogationCoef(
                        utils::get_item<float>(datas, "backpropagation_coef")
                        )
    {
        m_featureExtractor =
            utils::get_item<std::shared_ptr<IFeatureExtractor>>(datas, "feature_extractor");
        m_nbFeatures = m_featureExtractor->nb_features();

        m_layers = cv::Mat(2, 1, CV_32SC1);
        m_layers.row(0) = cv::Scalar(m_nbFeatures);
        m_layers.row(1) = cv::Scalar(1);

    }
    catch (std::bad_cast const &e) {
        std::cout << "bad_cast\t" << e.what() << std::endl;
    }
    catch (std::runtime_error const &e) {
        std::cout << "runtime_error\t" << e.what() << std::endl;
    }


    char
    NeuralNetworkClassifier::classify(cv::Mat &&featuresMatrix) const
    {
        cv::Mat classificationResult(1, m_nbOutputClasses, CV_32F);
        m_neuralNetwork->predict(featuresMatrix, classificationResult);
        return get_classification(classificationResult);
    }

    cv::Mat
    NeuralNetworkClassifier::get_data_matrix(std::vector<Dataset::Data> const &datas) const
    {
        std::cout << datas.size() << "\t" << m_nbFeatures <<  std::endl;
        cv::Mat value = cv::Mat::zeros(datas.size(), m_nbFeatures, CV_32F);
        std::cout << value << std::endl;
        unsigned int index = 0;
        for (auto const &i: datas) {
            auto sourceMat = m_featureExtractor->extract(std::get<1>(i));
            std::cout << sourceMat << std::endl;
            value.row(index++) = sourceMat;
        }
        std::cout << "value" << std::endl << value << std::endl;
        return value;
    }

    void
    NeuralNetworkClassifier::train(Dataset &&d)
    {
        using namespace cv::ml;
        using namespace cv;

        auto training_set = get_data_matrix(d.get_datas());
        // TODO: put the list of all output values
        auto training_set_classifications = cv::Mat();

        std::cout << m_layers << std::endl;

        ANN_MLP::Params p(
                m_layers, // Neural network typography
                ANN_MLP::SIGMOID_SYM, // Activation function
                0, // first activation function parameter BackprocCoef ?
                0, // second activation functon parameter
                TermCriteria(TermCriteria::EPS + TermCriteria::COUNT,
                            m_nbIterations, m_stopRate), // training stop condition
                ANN_MLP::Params::BACKPROP, // training algorithm
                0, // First parameter for the training method
                0 // Second parameter for the training method
                );
        training_set_classifications = cv::Mat::zeros(1, 0, CV_32F);
        m_neuralNetwork = ANN_MLP::create(p);
        int iterations = m_neuralNetwork->train(training_set, ROW_SAMPLE,
                                                cv::Mat::zeros(1, 1, CV_32F)
                );
    }

    void
    NeuralNetworkClassifier::serialize(std::string &&dest_path) const
    {
        if (m_neuralNetwork)
        {
            m_neuralNetwork->save(dest_path);
        }
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
