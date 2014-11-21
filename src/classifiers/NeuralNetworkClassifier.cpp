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
            utils::get_item<std::shared_ptr<IFeatureExtractor>>(datas,
                    "feature_extractor");
        m_nbFeatures = m_featureExtractor->nb_features();
        //m_neuralNetwork = new CvANN_MLP(m_layers, CvANN_MLP::SIGMOID_SYM,0.6,1);
    }
    catch (std::bad_cast const &e) {
        std::cerr << "bad_cast\t" << e.what() << std::endl;
    }
    catch (std::runtime_error const &e) {
        std::cerr << "runtime_error\t" << e.what() << std::endl;
    }

    inline std::string type2str(int type) {
        std::string r;

        uchar depth = type & CV_MAT_DEPTH_MASK;
        uchar chans = 1 + (type >> CV_CN_SHIFT);

        switch ( depth ) {
            case CV_8U:  r = "8U"; break;
            case CV_8S:  r = "8S"; break;
            case CV_16U: r = "16U"; break;
            case CV_16S: r = "16S"; break;
            case CV_32S: r = "32S"; break;
            case CV_32F: r = "32F"; break;
            case CV_64F: r = "64F"; break;
            default:     r = "User"; break;
        }

        r += "C";
        r += (chans+'0');

        return r;
    }

    std::string
    NeuralNetworkClassifier::classify(cv::Mat &&featuresMatrix,
                                      Dataset const &dataset) const
    {
        cv::Mat classificationResult(1, dataset.get_nb_output(), featuresMatrix.type());
        //std::cout << type2str(classificationResult.type()) << std::endl;
        //std::cout << type2str(featuresMatrix.type()) << std::endl;
        //std::cout << classificationResult.rows << "\t" << classificationResult.cols << std::endl;
        //std::cout << featuresMatrix.rows << "\t" << featuresMatrix.cols << std::endl;


        m_neuralNetwork->predict(featuresMatrix, classificationResult);
        classificationResult.at<double>(0, 0);
        auto maxIndex = get_classification(classificationResult,
                                           dataset.get_nb_output());
        auto value = dataset.get_value(maxIndex);
        return value;
    }

    int
    NeuralNetworkClassifier::get_classification(cv::Mat const &classificationResult,
            std::size_t nbOutputClasses
            ) const
    {
        int maxIndex = 0;
        float value=0.0f;
        float maxValue = classificationResult.at<double>(0, 0);
        for (unsigned int index = 1; index < nbOutputClasses; ++index) {
            value = classificationResult.at<double>(0, index);
            //std::cout << value << std::endl;
            if (value > maxValue) {
                //std::cout << maxValue << std::endl;
                maxValue = value;
                maxIndex = index;
            }
        }
        return maxIndex;
    }

    cv::Mat
    NeuralNetworkClassifier::get_data_matrix(std::vector<Dataset::Data> const &datas) const
    {
        cv::Mat value = cv::Mat::zeros(datas.size(), m_nbFeatures, CV_32F);
        unsigned int index = 0;
        for (auto const &i: datas) {
            auto sourceMat = m_featureExtractor->extract(std::get<1>(i));
            sourceMat.copyTo(value.row(index++));
        }
        return value;
    }

    cv::Mat
    NeuralNetworkClassifier::get_classification_matrix(cv::Mat const &mat,
                                                       Dataset &&dataset
                                                       )
    {
        auto size = mat.size();
        // TODO: add zeros
        auto tmp = cv::Mat::zeros(size.height, dataset.get_nb_output(), CV_32F);
        auto value = cv::Mat(tmp);
        for (ssize_t i = 0; i < size.height; ++i) {
            value.at<float>(i, dataset.get_output_for(i)) = 1.0;
        }
        return value;
    }

    void
    NeuralNetworkClassifier::train(Dataset &&d)
    {
        using namespace cv;

        m_training_set = get_data_matrix(d.get_datas());
        m_training_set_classifications = get_classification_matrix(m_training_set,
                                                                   std::move(d));
        CvANN_MLP_TrainParams p(
                //m_layers, // Neural network typography
                //ANN_MLP::SIGMOID_SYM, // Activation function
                //0.01, // first activation function parameter BackprocCoef ?
                //0.01, // second activation functon parameter
                cvTermCriteria(TermCriteria::EPS + TermCriteria::COUNT,
                             m_nbIterations, m_stopRate), // training stop condition
                CvANN_MLP_TrainParams::BACKPROP, // training algorithm
                0.01, // First parameter for the training method
                0.01 // Second parameter for the training method
                );

        m_layers = cv::Mat(5, 1, CV_32S);
        m_layers.at<int>(0, 0) = m_featureExtractor->nb_features();
        m_layers.at<int>(1, 0) = 40;
        m_layers.at<int>(2, 0) = 40;
        m_layers.at<int>(3, 0) = 40;
        m_layers.at<int>(4, 0) = d.get_nb_output();
        m_neuralNetwork = new CvANN_MLP(m_layers, CvANN_MLP::SIGMOID_SYM,
                0.0,0);
        int iterations = m_neuralNetwork->train(
                m_training_set,
                m_training_set_classifications,
                cv::Mat(),
                cv::Mat(),
                p
                );
        std::cout << "iterations\t" << iterations << std::endl;
    }

    void
    NeuralNetworkClassifier::serialize(std::string &&dest_path) const
    {

        if (m_neuralNetwork) {
            CvFileStorage* storage = cvOpenFileStorage(dest_path.c_str(),
                     0, CV_STORAGE_WRITE);
            m_neuralNetwork->write(storage, "SimpleOcr");
            cvReleaseFileStorage(&storage);
        }
    }

    void
    NeuralNetworkClassifier::unserialize(std::string const &filePath)
    {
        CvANN_MLP neuralNetwork;
        neuralNetwork.load(filePath.c_str(), "SimpleOcr");
        m_neuralNetwork = new CvANN_MLP(neuralNetwork);
    }

    char const *
    NeuralNetworkClassifier::name() const
    {
        return "NeuralNetworkClassifier";
    }
} /* namespace ocr */

extern "C"
ocr::NeuralNetworkClassifier *
constructor(utils::Json::Map const &m)
{
    return new ocr::NeuralNetworkClassifier(m);
}
