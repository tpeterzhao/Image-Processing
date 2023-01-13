#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <sstream>
#include <math.h>
#include <opencv2/opencv.hpp>
// #include "opencv2/core/core.hpp"
// #include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include <opencv2/imgcodecs.hpp>

class utility
{
	public:
		utility();
		virtual ~utility();
		static std::string intToString(int number);
		static int checkValue(int value);
		static void addGrey(image &src, image &tgt, int value);
		static void binarize(image &src, image &tgt, int threshold);
		static void scale(image &src, image &tgt, float ratio);
		static void dualthres(image &src, image &tgt, int t, int v1, int v2);
		static void reg2dsmooth(image &src, image &tgt, int ws);
		static void sep2dsmooth(image &src, image &tgt, int ws);
		static void colorbright(image &src, image &tgt, float dr, float dg, float db);
		static void colorvisual(image &src, image &tgt, int thres, int v1);
		static void histostretch(image &src, image &tgt, int a, int b);		
		static void althistostretch(image &src, image &tgt, int a, int b);
		static void histothres(image &src, image &tgt, int thres, char borf, int a, int b);
		static void colorstretch(image &src, image &tgt, char channel, int a, int b);
		static void rgbtohsi(int r, int g, int b, int &h, int &s, int &i);
		static void hsitorgb(int h, int s, int i, int &r, int &g, int &b);
		static void hsistretch(image &src, image &tgt, char channel, int a, int b);
		static void sobel3(image &src, image &tgt, image &dir);
		static void sobel5(image &src, image &tgt, image &dir);
		static void binaryedge(image &src, image &tgt, int th, int tl, image &ver, image &hor);
		static void sobelcv(image &src, image &tgt, int size);
		static void gausobel(image &src, image &tgt, float sigma);
};

#endif

