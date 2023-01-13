/************************************************************
 *															*
 * This sample project include three functions:				*
 * 1. Add intensity for gray-level image.					*
 *    Input: source image, output image name, value			*
 *															*
 * 2. Image thresholding: pixels will become black if the	*
 *    intensity is below the threshold, and white if above	*
 *    or equal the threhold.								*
 *    Input: source image, output image name, threshold		*
 *															*
 * 3. Image scaling: reduction/expansion of 2 for 			*
 *    the width and length. This project uses averaging 	*
 *    technique for reduction and pixel replication			*
 *    technique for expansion.								*
 *    Input: source image, output image name, scale factor	*
 *															*
 ************************************************************/

#include "../iptools/core.h"
#include <strings.h>
#include <string.h>

using namespace std;

#define MAXLEN 256
struct ROI
{
	/* data */
	int x1, y1, sx1, sy1;
	cv::Mat r;
	bool color = false;
};


int main (int argc, char** argv)
{
	cv::Mat src, tgt, extra1, extra2;
	FILE *fp;
	int numROI;
	char str[MAXLEN];
	char infile[MAXLEN];
	char outfile[MAXLEN];
	char *pch;
	if ((fp = fopen(argv[1],"r")) == NULL) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);
	}

	while(fgets(str,MAXLEN,fp) != NULL) {
		// read name of original input image 
		pch = strtok(str, " ");
		strcpy(infile,pch);
		src = cv::imread(infile);
		// read name of output image
		pch = strtok(NULL, " ");
		strcpy(outfile, pch);
		// read the number of ROIs
		pch = strtok(NULL, " ");
		numROI = atoi(pch);

		bool bin_flag = false;

		struct ROI rois[numROI];
		// read ROIs
		for(int i=0; i<numROI; i++){
			// read roi regions
			int x1, y1, sx1, sy1;
			pch = strtok(NULL, " ");
			x1 = atoi(pch);
			pch = strtok(NULL, " ");
			y1 = atoi(pch);
			pch = strtok(NULL, " ");
			sx1 = atoi(pch);
			pch = strtok(NULL, " ");
			sy1 = atoi(pch);
			rois[i].x1 = x1;
			rois[i].y1 = y1;
			rois[i].sx1 = sx1;
			rois[i].sy1 = sy1;

			if(x1<0 || y1<0 || sx1<0 || sy1<0 || (x1+sx1) >= src.cols || (y1+sy1) >= src.rows){
				printf("ROI is not in bounds.\n");
				return 1;
			}
			
			// read command
			pch = strtok(NULL, " ");
			if (strncasecmp(pch,"cannycv",7)==0){
				cv::Mat I = cv::imread(infile,cv::IMREAD_GRAYSCALE);
				cv::Mat R = I(cv::Rect(x1,y1,sx1,sy1));
				cv::Mat canny;
				cv::Canny(R, canny, 20, 80);
				canny.copyTo(rois[i].r);
			}else if(strncasecmp(pch, "gaucv", 5)==0){
				cv::Mat I = cv::imread(infile,cv::IMREAD_GRAYSCALE);
				cv::Mat R = I(cv::Rect(x1,y1,sx1,sy1));
				cv::Mat gau;
				cv::GaussianBlur(R,gau, cv::Size(7,7), 0);
				gau.copyTo(rois[i].r);
			}
			else if (strncasecmp(pch,"histostretchcv",14)==0){
				float low = 50;
				float high = 150;
				cv::Mat I = cv::imread(infile,cv::IMREAD_GRAYSCALE);
				cv::Mat R = I(cv::Rect(x1,y1,sx1,sy1));
				cv::Mat equal, gra, mul, stretched;
				cv::equalizeHist(R, equal);
				cv::multiply(equal, (high-low)/256, mul);
				cv::add(mul, low, stretched);
				stretched.copyTo(rois[i].r);
			}else if (strncasecmp(pch,"histoequalcv",12)==0){
				cv::Mat I = cv::imread(infile,cv::IMREAD_GRAYSCALE);
				cv::Mat R = I(cv::Rect(x1,y1,sx1,sy1));
				cv::Mat gra;
				cv::equalizeHist(R, gra);
				gra.copyTo(rois[i].r);
			}else if (strncasecmp(pch,"houghtrans",10)==0){
				cv::Mat I = cv::imread(infile,cv::IMREAD_GRAYSCALE);
				cv::Mat R = I(cv::Rect(x1,y1,sx1,sy1));
				cv::Mat canny, gau;
				cv::GaussianBlur(R, gau, cv::Size(7,7), 0);
				cv::Canny(gau, canny, 20, 80);
				vector<cv::Vec3f> circles;
				HoughCircles(canny, circles, cv::HOUGH_GRADIENT,
                 2, canny.rows/4);
				for(int i=0; i<circles.size(); i++){
					cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
					int radius = cvRound(circles[i][2]);
					cv::circle(canny, center, radius, 255, 3, cv::LINE_AA);
				}
				canny.copyTo(rois[i].r);
			}else if (strncasecmp(pch,"hesobel",7)==0){
				cv::Mat I = cv::imread(infile,cv::IMREAD_GRAYSCALE);
				cv::Mat R = I(cv::Rect(x1,y1,sx1,sy1));
				cv::Mat he, heso, noheso, sub;
				cv::equalizeHist(R,he);
				cv::Sobel(he, heso, CV_64F, 1, 1);
				cv::Sobel(R, noheso, CV_64F, 1, 1);
				cv::subtract(heso, noheso, sub);
				sub.copyTo(rois[i].r);
			}else if (strncasecmp(pch,"hecanny",7)==0){
				cv::Mat I = cv::imread(infile,cv::IMREAD_GRAYSCALE);
				cv::Mat R = I(cv::Rect(x1,y1,sx1,sy1));
				cv::Mat he, hecanny, nohecanny, sub;
				cv::equalizeHist(R,he);
				cv::Canny(he, hecanny, 150, 100);
				cv::Canny(R, nohecanny, 150, 100);
				cv::subtract(hecanny, nohecanny, sub);
				sub.copyTo(rois[i].r);
			}else if (strncasecmp(pch,"qrdecode",8)==0){
				cv::Mat I = cv::imread(infile,cv::IMREAD_COLOR);
				cv::Mat R = I(cv::Rect(x1, y1, sx1, sy1));
				cv::Mat qr, eq;

				cv::cvtColor(R, qr, cv::COLOR_RGB2GRAY);
				cv::threshold(qr, eq, 100, 255,cv::THRESH_BINARY);
				cv::QRCodeDetector decoder = cv::QRCodeDetector();
				std::string data = decoder.detectAndDecode(eq);
				cv::imwrite("gray.png", eq);
				std::cout<<data<<"\n";
				R.copyTo(rois[i].r);
				rois[i].color = true;
			}
			else {
				printf("No function: %s\n", pch);
				return 1;
			}		
		}
		// read cv image to copy rois back to
		cv::Mat I;
		if(rois[0].color){
			I = cv::imread(infile,cv::IMREAD_COLOR);
		} else{
			I = cv::imread(infile, cv::IMREAD_GRAYSCALE);
		}

		// copy rois back to full image
		for(int i=numROI-1; i>=0; i--){
			rois[i].r.copyTo(I(cv::Rect(rois[i].x1,rois[i].y1,rois[i].sx1,rois[i].sy1)));
		}
		cv::imwrite(outfile,I);
	}
	fclose(fp);
	return 0;
}

