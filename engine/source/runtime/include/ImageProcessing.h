#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
namespace EasyEngine {
	using namespace cv;
	using namespace std;

	class ImageProcessing
	{
	public:
		static Mat readImageByPath(const string& imagePath, int colorFlag = IMREAD_UNCHANGED,bool inverseY = false);
	};

}