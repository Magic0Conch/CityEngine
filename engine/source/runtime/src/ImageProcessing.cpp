#include "../include/ImageProcessing.h"
#include <opencv2/core.hpp>
namespace EasyEngine {
Mat ImageProcessing::readImageByPath(const string& imagePath, int colorFlag,bool inverseY) {
	Mat img = imread(imagePath, colorFlag);
	if (inverseY) {
		cv::flip(img, img, 0);
	}
	if (img.empty()) {
		cout << "read image failed: " + imagePath << endl;
	}
	return img;
}
}