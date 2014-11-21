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


    std::string
    NeuralNetworkClassifier::classify(cv::Mat &&featuresMatrix,
                                      Dataset const &dataset) const
    {
        cv::Mat classificationResult(1, m_nbOutputClasses, CV_32F);
        m_neuralNetwork->predict(featuresMatrix, classificationResult);
        // return 'c';
        return dataset.get_value(get_classification(classificationResult));
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
        //std::cout << "value\t" << value << std::endl;
        return value;
    }

    cv::Mat
    NeuralNetworkClassifier::get_classification_matrix(cv::Mat const &mat,
                                                       Dataset &&dataset
                                                       )
    {
        auto size = mat.size();
        // TODO: add zeros
        auto value = cv::Mat(size.height, dataset.get_nb_output(), CV_32F);
        for (ssize_t i = 0; i < size.height; ++i) {
            //std::cout << i << "\t" << dataset.get_output_for(i) << std::endl;
            value.at<float>(i, dataset.get_output_for(i)) = 1.0;
        }
        //std::cout << "value\t" << value << std::endl;
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

        m_layers = cv::Mat(4, 1, CV_32S);
        m_layers.at<int>(0, 0) = m_featureExtractor->nb_features();
        m_layers.at<int>(1, 0) = 16;
        m_layers.at<int>(2, 0) = 16;
        m_layers.at<int>(3, 0) = d.get_nb_output();
        std::cout << m_layers << std::endl;
        m_neuralNetwork = new CvANN_MLP(m_layers, CvANN_MLP::SIGMOID_SYM,0.6,1);
        //m_neuralNetwork = ANN_MLP::create(p);
        std::cout << "avant"  << std::endl;
        std::cout << m_training_set << std::endl;
        std::cout << m_training_set_classifications << std::endl;
        int iterations = m_neuralNetwork->train(
                m_training_set,
                m_training_set_classifications,
                cv::Mat(),
                cv::Mat(),
                p
                );
        std::cout << "apres" << std::endl;
        std::cout << iterations << std::endl;
        //serialize("./test.xml");
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
    }

    char const *
    NeuralNetworkClassifier::name() const
    {
        return "NeuralNetworkClassifier";
    }

    int
    NeuralNetworkClassifier::get_classification(cv::Mat const &classificationResult) const
    {
        int maxIndex = 0;
        float value=0.0f;
        float maxValue = classificationResult.at<float>(0, 0);
        for (int index = 1; index < m_nbOutputClasses; ++index) {
            classificationResult.at<float>(0, index);
            if (value > maxValue) {
                maxValue = value;
                maxIndex = index;
            }
        }
        return maxIndex;
    }

} /* namespace ocr */

extern "C"
ocr::NeuralNetworkClassifier *
constructor(utils::Json::Map const &m)
{
    return new ocr::NeuralNetworkClassifier(m);
}
