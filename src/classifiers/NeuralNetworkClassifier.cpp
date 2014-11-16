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

    }
    catch (std::bad_cast const &e) {
        std::cerr << "bad_cast\t" << e.what() << std::endl;
    }
    catch (std::runtime_error const &e) {
        std::cerr << "runtime_error\t" << e.what() << std::endl;
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
        cv::Mat value = cv::Mat::zeros(datas.size(), m_nbFeatures, CV_32F);
        unsigned int index = 0;
        for (auto const &i: datas) {
            auto sourceMat = m_featureExtractor->extract(std::get<1>(i));
            auto tmp = sourceMat.size();
            //std::cout << sourceMat<< std::endl;
            //std::cout << value.row(index) << std::endl;
            sourceMat.copyTo(value.row(index++));
            //std::cout << sourceMat.col(0).at<float>(0) << std::endl;
            //value.row(index++) = sourceMat.col(0);
        }
        //std::cout << "value\t" << value << std::endl;
        return value;
    }

    cv::Mat
    NeuralNetworkClassifier::get_classification_matrix(cv::Mat const &mat,
                                                       int nb_output_classification)
    {
        auto size = mat.size();
        auto value = cv::Mat::zeros(size.height, nb_output_classification, CV_32F);
        return value;
    }

    void
    NeuralNetworkClassifier::train(Dataset &&d)
    {
        using namespace cv::ml;
        using namespace cv;

        auto training_set = get_data_matrix(d.get_datas());
        auto training_set_classifications = get_classification_matrix(training_set,
                                                                      d.get_nb_output());
        // TODO: put the list of all output values
        //std::cout << m_layers << std::endl;
        //auto training_set_classifications = cv::Mat::zeros(2, 1, CV_32F);

        m_layers = cv::Mat(3, 1, CV_32SC1);
        m_layers.row(0) = cv::Scalar(m_nbFeatures);
        m_layers.row(1) = cv::Scalar(9);
        m_layers.row(2) = cv::Scalar(d.get_nb_output());


        ANN_MLP::Params p(
                m_layers, // Neural network typography
                ANN_MLP::SIGMOID_SYM, // Activation function
                0.001, // first activation function parameter BackprocCoef ?
                0.001, // second activation functon parameter
                TermCriteria(TermCriteria::EPS + TermCriteria::COUNT,
                             m_nbIterations, m_stopRate), // training stop condition
                ANN_MLP::Params::BACKPROP, // training algorithm
                0.0001, // First parameter for the training method
                0.0001 // Second parameter for the training method
                );
        m_neuralNetwork = ANN_MLP::create(p);
        std::cout << "avant"  << std::endl;
        int iterations = m_neuralNetwork->train(training_set, ROW_SAMPLE,
                                                training_set_classifications
                );
        std::cout << "apres" << std::endl;
        std::cout << iterations << std::endl;
        serialize("./test.xml");
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