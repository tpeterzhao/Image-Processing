This software is architectured as follows. This software can work on grad server.

iptools -This folder hosts the files that are compiled into a static library. 
	image - This folder hosts the files that define an image.
	utility- this folder hosts the files that students store their implemented algorithms.
	
lib- This folder hosts the static libraries associated with this software.

project- This folder hosts the files that will be compiled into executables.
	bin- This folder hosts the binary executables created in the project directory.



*** INSTALATION ***

On Linux

Enter the project directory in terminal and run make

As a result you should get iptool in project/bin directory.

*** FUNCTIONS ***

1. histostretchcv function: stretch the histogram to the range of 50 to 150
2. histoequalcv function: stretch the histogram to the range of 0 to 255
3. houghtrans function: perform canny operation then perform hough tranformation, then smooth with Gaussian smoothing.
	- Canny operation default thresholding values: 20, 80
	- Gaussian smoothing default size: 7x7
4. gaucv function: Gaussian smoothing with 7x7 window
5. cannycv function: canny edge detection with thresholding of 20 and 80
6. hesobel function: perform histogram equalization followed by Sobel edge detection.
	- outputs the subtraction of Sobel edge detection of histogram equalization image and the Sobel edge detection of the original image.
7. hecanny function: perform histogram equalization followed by Canny edge detection.
	- outputs the subtraction of Canny edge detection of histogram equalization image and the Canny edge detection of the original image.
8. qrcode: detect and decode qr code.
	- stil need to input the output file name, even though the output image is just a copy of input image.
	- detect and decodes qr code and output the decoded message in console.

*** PARAMETERS FILE ***

There are for parameters:
1. the input file name;
2. the output file name;
3. the number of ROIs
4. the initial row of ROI
5. the initial column of ROI
6. total number of pixels in the x axis
7. total number of pixels in the y axis
8. the name of the filter.
p1...px parameters needed for function


*** Run the program: ./iptool parameters.txt
