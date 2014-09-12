#pragma once


#if 0

namespace ocr {


    // Will be implemented latter
    //      Should detect the interesting zone in the image
    class PatternFinder {
    public:
        PatternFinder();
        ~PatternFinder();

        std::vector<cv::Mat> &&get_zones(Image img);
    };
}

#endif
