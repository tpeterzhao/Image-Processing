#include "utility.h"

#define MAXRGB 255
#define MINRGB 0

std::string utility::intToString(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int utility::checkValue(int value)
{
	if (value > MAXRGB)
		return MAXRGB;
	if (value < MINRGB)
		return MINRGB;
	return value;
}

/*-----------------------------------------------------------------------**/
void utility::addGrey(image &src, image &tgt, int value)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++){
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			tgt.setPixel(i, j,checkValue(src.getPixel(i,j)+value)); 
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::binarize(image &src, image &tgt, int threshold)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if (src.getPixel(i,j) < threshold)
				tgt.setPixel(i,j,MINRGB);
			else
				tgt.setPixel(i,j,MAXRGB);
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::scale(image &src, image &tgt, float ratio)
{
	// int rows = (int)((float)src.getNumberOfRows() * ratio);
	// int cols  = (int)((float)src.getNumberOfColumns() * ratio);
	// tgt.resize(rows, cols);
	for (int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{	
			/* Map the pixel of new image back to original image */
			int i2 = (int)floor((float)i/ratio);
			int j2 = (int)floor((float)j/ratio);
			if (ratio == 2) {
				/* Directly copy the value */
				tgt.setPixel(i,j,checkValue(src.getPixel(i2,j2)));
			}

			if (ratio == 0.5) {
				/* Average the values of four pixels */
				int value = src.getPixel(i2,j2) + src.getPixel(i2,j2+1) + src.getPixel(i2+1,j2) + src.getPixel(i2+1,j2+1);
				tgt.setPixel(i,j,checkValue(value/4));
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
// implement dualthresfunction

void utility::dualthres(image &src, image &tgt, int t, int v1, int v2){
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for(int i=0; i<src.getNumberOfRows(); i++){
		for (int j=0; j<src.getNumberOfColumns(); j++){
			if(src.getPixel(i,j)>t){
				tgt.setPixel(i,j,checkValue(src.getPixel(i,j)+v1));
			} else if(src.getPixel(i,j)<t){
				tgt.setPixel(i,j,checkValue(src.getPixel(i,j)-v2));
			}
		}
	}
}

// implement smooth function
void utility::reg2dsmooth(image  &src, image &tgt, int ws){
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for(int i=0; i<src.getNumberOfRows(); i++){
		for(int j=0; j<src.getNumberOfColumns(); j++){
			int sum = 0;
			// if border pixel, just copy the pixel
			if(!src.isInbounds(i-(ws-1)/2,j-(ws-1)/2) || !src.isInbounds(i+(ws-1)/2, j+(ws-1)/2)){
				tgt.setPixel(i,j,src.getPixel(i,j));
			} else{
				for(int x= (-(ws-1)/2); x<=(ws-1)/2; x++){
					for(int y= (-(ws-1)/2); y<=(ws-1)/2; y++){
						sum += src.getPixel(i+x, j+y);
					}
				}
				tgt.setPixel(i,j,(int)floor((float)sum/(ws*ws)));
			}
		}
	}
}

// implement seperate smoothing
void utility::sep2dsmooth(image &src, image &tgt, int ws){
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for(int i=0; i<src.getNumberOfRows(); i++){
		for(int j=0; j<src.getNumberOfColumns(); j++){
			// if border pixel, just copy the pixel
			if(!src.isInbounds(i-(ws-1)/2,j-(ws-1)/2) || !src.isInbounds(i+(ws-1)/2, j+(ws-1)/2)){
				tgt.setPixel(i,j,src.getPixel(i,j));
			} else 
			{
				if(i == (ws-1)/2){
					int sum = 0;
					for(int x= (-(ws-1)/2); x<=(ws-1)/2; x++){
						sum += src.getPixel(i+x, j);
					}
					tgt.setPixel(i,j,floor(sum/ws));
				} else{
					int p = tgt.getPixel(i-1,j) - src.getPixel(i-1-(ws-1)/2, j)/ws + src.getPixel(i+(ws-1)/2, j)/ws;
					tgt.setPixel(i,j, p);
				}	
				if(j == (ws-1)/2){
					int sum = 0;
					for(int x= (-(ws-1)/2); x<=(ws-1)/2; x++){
						sum += src.getPixel(i, j+x);
					}
					tgt.setPixel(i,j,floor(sum/ws));
				} else{
					int p = tgt.getPixel(i,j-1) - src.getPixel(i, j-1-(ws-1)/2)/ws + src.getPixel(i, j+(ws-1)/2)/ws;
					tgt.setPixel(i,j,p);
				}
			}
		}
	}
}

void utility::colorbright(image &src, image &tgt, float dr, float dg, float db){

	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	// red channel
	for (int i=0; i<src.getNumberOfRows(); i++){
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			tgt.setPixel(i,j,RED,checkValue(floor(src.getPixel(i,j,RED)*dr))); 
		}
	}	
	// green channel
	for (int i=0; i<src.getNumberOfRows(); i++){
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			tgt.setPixel(i,j,GREEN,checkValue(floor(src.getPixel(i,j,GREEN)*dg))); 
		}
	}	
	// blue channel
	for (int i=0; i<src.getNumberOfRows(); i++){
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			tgt.setPixel(i,j,BLUE,checkValue(floor(src.getPixel(i,j,BLUE)*db))); 
		}
	}
}

void utility::colorvisual(image &src, image &tgt, int thres, int v1){

	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if (abs(v1 - src.getPixel(i,j))<thres){
				tgt.setPixel(i,j,RED,255);
			}
			else
				tgt.setPixel(i,j,src.getPixel(i,j));
		}
	}	
}

void utility::histostretch(image &src, image &tgt, int a, int b){
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	// a, b are the range of intensity.
	int hist_max = 0;
	// generate histogram
	int hist[256] = {};
	for(int i=0; i<src.getNumberOfRows(); i++){// iterate rows
		for(int j=0; j<src.getNumberOfColumns(); j++){// iterate columns
			int intensity = src.getPixel(i,j);
			hist[intensity]++;
			if(hist[intensity]>hist_max){
				hist_max = hist[intensity];
			}
		}
	}

	// find min
	int min = 0;
	for(int i=0; i<256; i++){
		if(hist[i]!=0){
			min = i;
			break;
		}
	}
	// find max
	int max = 255;
	for(int i=255; i>=0; i--){
		if(hist[i]!=0){
			max = i;
			break;
		}
	}	

	image hist1(256,256);
	for(int i=0; i<256; i++){
		for(int j=0; j<=floor(hist[i]/float(hist_max)*255); j++){
			hist1.setPixel(255-j,i,255);
		}
	}
	
	for(int i=0; i<src.getNumberOfRows(); i++){// iterate rows
		for(int j=0; j<src.getNumberOfColumns(); j++){// iterate columns
			int intensity = src.getPixel(i,j);
			float value;
			value =(intensity-min)/float(max-min)*(b-a) + a;
			intensity = floor(value);
			tgt.setPixel(i,j,checkValue(intensity));
		}
	}

	// generate histogram afterward
	int after_max = 0;
	// generate histogram
	int after[256] = {};
	for(int i=0; i<tgt.getNumberOfRows(); i++){// iterate rows
		for(int j=0; j<tgt.getNumberOfColumns(); j++){// iterate columns
			int intensity = tgt.getPixel(i,j);
			after[intensity]++;
			if(after[intensity]>after_max){
				after_max = after[intensity];
			}
		}
	}

	image hist2(256,256);
	for(int i=0; i<256; i++){
		for(int j=0; j<=floor(after[i]/float(after_max)*255); j++){
			hist2.setPixel(255-j,i,255);
		}
	}
}

void utility::althistostretch(image &src, image &tgt, int a, int b){
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	// a, b are the range of intensity.
	tgt.copyImage(src);
	int hist_max = 0;
	// generate histogram
	int hist[256] = {};
	for(int i=0; i<src.getNumberOfRows(); i++){// iterate rows
		for(int j=0; j<src.getNumberOfColumns(); j++){// iterate columns
			int intensity = src.getPixel(i,j);
			hist[intensity]++;
			if(hist[intensity]>hist_max){
				hist_max = hist[intensity];
			}
		}
	}

	// find min
	float min = 0.0;
	for(int i=0; i<256; i++){
		if(hist[i]!=0){
			min = i;
			break;
		}
	}
	min = floor(1.05 * min);
	// find max
	float max = 255.0;
	for(int i=255; i>=0; i--){
		if(hist[i]!=0){
			max = i;
			break;
		}
	}	
	max = floor(0.95*max);


	image hist1(256,256);
	for(int i=0; i<256; i++){
		for(int j=0; j<=floor(hist[i]/float(hist_max)*255); j++){
			hist1.setPixel(255-j,i,255);
		}
	}
	hist1.save("althist_before.pgm");
	
	// histogram stretching
	for(int i=0; i<src.getNumberOfRows(); i++){// iterate rows
		for(int j=0; j<src.getNumberOfColumns(); j++){// iterate columns
			int intensity = src.getPixel(i,j);
			float value;
			value =(intensity-min)/float(max-min)*(b-a) + a;
			intensity = floor(value);
			tgt.setPixel(i,j,checkValue(intensity));
		}
	}

	// generate histogram afterward
	int after_max = 0;
	// generate histogram
	int after[256] = {};
	for(int i=0; i<tgt.getNumberOfRows(); i++){// iterate rows
		for(int j=0; j<tgt.getNumberOfColumns(); j++){// iterate columns
			int intensity = tgt.getPixel(i,j);
			after[intensity]++;
			if(after[intensity]>after_max){
				after_max = after[intensity];
			}
		}
	}

	image hist2(256,256);
	for(int i=0; i<256; i++){
		for(int j=0; j<=floor(after[i]/float(after_max)*255); j++){
			hist2.setPixel(255-j,i,255);
		}
	}
	hist2.save("althist_after.pgm");
}

void utility::histothres(image &src, image &tgt, int thres, char borf, int a, int b){
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	tgt.copyImage(src);
	image thres_map;
	thres_map.resize(src.getNumberOfRows(),src.getNumberOfColumns());
	// iterate through the image and label each pixel
	for(int i=0; i<src.getNumberOfRows(); i++){
		for(int j=0; j<src.getNumberOfColumns(); j++){
			// background
			if(src.getPixel(i,j)<=thres){
				thres_map.setPixel(i,j,0);
				// mask background
				if(borf=='F'){
					tgt.setPixel(i,j,255);
				}
			} else{
				// foreground
				thres_map.setPixel(i,j,255);

				// mask foreground if user chose background
				if(borf=='B'){
					tgt.setPixel(i,j, 0);
				}
			}
		}
	}

	image stretched;
	histostretch(tgt, stretched, a, b);
	for(int i=0; i<src.getNumberOfRows(); i++){
		for(int j=0; j<src.getNumberOfColumns(); j++){
			if(borf=='B'){
				if(thres_map.getPixel(i,j)==0){
					tgt.setPixel(i,j,stretched.getPixel(i,j));
				} else{
					tgt.setPixel(i,j,src.getPixel(i,j));
				}
			} else{
				if(thres_map.getPixel(i,j)==255){
					tgt.setPixel(i,j,stretched.getPixel(i,j));
				} else{
					tgt.setPixel(i,j,src.getPixel(i,j));
				}
			}
		}
	}
	
}

void utility::colorstretch(image &src, image &tgt, char channel, int a, int b){
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());	
	tgt.copyImage(src);
	image color(src.getNumberOfRows(), src.getNumberOfColumns());

	// get color channel image
	for(int i=0; i<src.getNumberOfRows(); i++){
		for(int j=0; j<src.getNumberOfColumns(); j++){
			if(channel == 'R'){
				color.setPixel(i,j,src.getPixel(i,j,RED));
			} else
			if(channel == 'G'){
				color.setPixel(i,j,src.getPixel(i,j,GREEN));
			} else
			if(channel == 'B'){
				color.setPixel(i,j,src.getPixel(i,j,BLUE));
			}
		}
	}
	image stretched( src.getNumberOfRows(), src.getNumberOfColumns());
	histostretch( color, stretched, a, b);
	for(int i=0; i<src.getNumberOfRows(); i++){
		for(int j=0; j<src.getNumberOfColumns(); j++){
			if(channel == 'R'){
				tgt.setPixel(i,j, RED, stretched.getPixel(i,j));
			} else
			if(channel == 'G'){
				tgt.setPixel(i,j, GREEN, stretched.getPixel(i,j));
			} else
			if(channel == 'B'){
				tgt.setPixel(i,j, BLUE, stretched.getPixel(i,j));
			}
		}
	}
}

void utility::rgbtohsi(int r, int g, int b, int &h, int &s, int &i){
	// normalization
	float nr = r/float(r+g+b);
	float ng = g/float(r+g+b);
	float nb = b/float(r+g+b);

	float fh, fs, fi;
	if(nb <= ng){
		fh = acos((0.5*((nr-ng)+(nr-nb)))/pow((pow((nr-ng),2)+(nr-nb)*(ng-nb)),0.5));
		// printf("small hue: %f\n", fh);
	} else{
		fh = 2*M_PI - acos((0.5*((nr-ng)+(nr-nb)))/(pow((pow((nr-ng),2)+(nr-nb)*(ng-nb)),0.5)));
		// printf("large hue: %f\n", fh);
	}

	fs = 1-3* min(nr,min(ng,nb));
	fi = (r+g+b)/float(3*255);
	// printf("hue: %f\n", fh*180/M_PI);
	h = floor(fh*180/M_PI);
	// printf("int hue: %d\n", h);
	s = floor(fs*100);
	i = floor(fi* 255);
}

void utility::hsitorgb(int h, int s, int i, int &r, int &g, int &b){
	// normalization
	float nh = h*M_PI/(float)180;
	float ns = s/(float)100;
	float ni = i/(float)255;

	float x = ni*(1-ns);
	float y = ni*(1+ns*cos(nh)/cos(M_PI/3-nh));
	float z = 3*ni - (x+y);

	if(nh>=0 && nh< 2*M_PI/3){
		b = floor(255*(ni*(1-ns)));
		r = floor(255*(ni*(1+ns*cos(nh)/cos(M_PI/3-nh))));
		// printf("nh: %f\n", nh-M_PI/2);

		g = floor(255*(3*ni-(b + r)));
	}
	if(nh>=2*M_PI/3 && nh<4*M_PI/3){
		nh = nh - 2*M_PI/3;
		r = floor(255*(ni*(1-ns)));
		g = floor(255*(ni*(1+ns*cos(nh)/cos(M_PI/3-nh))));
		// printf("g: %f\n", cos(nh));
		b = floor(255*(3*ni-(r + g)));
	}
	if(nh>=4*M_PI/3 && nh<2*M_PI){
		nh = nh-4*M_PI/3;
		g = floor(255*(ni*(1-ns)));
		b = floor(255*(ni*(1+ns*cos(nh)/cos(M_PI/3-nh))));
		// printf("b: %f\n", cos(nh));
		r = floor(255*(3*ni-(g + b)));
	}
}

void utility::hsistretch(image &src, image &tgt, char channel, int a, int b){
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());

	image i_map(src.getNumberOfRows(), src.getNumberOfColumns());
	image h_map(src.getNumberOfRows(), src.getNumberOfColumns());
	image s_map(src.getNumberOfRows(), src.getNumberOfColumns());

	// generate grey level image for h, s, i
	for(int x=0; x<src.getNumberOfRows();x++){
		for(int y=0; y<src.getNumberOfColumns(); y++){
			int r,g,b;
			r = src.getPixel(x,y,RED);
			g = src.getPixel(x,y,GREEN);
			b = src.getPixel(x,y,BLUE);
			int h,s,i;
			rgbtohsi(r,g,b,h,s,i);
			i_map.setPixel(x,y,i);
			h_map.setPixel(x,y,h);
			s_map.setPixel(x,y,s);
		}
	}

	image stretched(src.getNumberOfRows(), src.getNumberOfColumns());
	histostretch(i_map, stretched, a, b);
	
	for(int x=0; x<src.getNumberOfRows();x++){
		for(int y=0; y<src.getNumberOfColumns(); y++){
			int r,g,b;
			int h,s,i;
			i = stretched.getPixel(x,y);
			h = h_map.getPixel(x,y);
			s = s_map.getPixel(x,y);
			hsitorgb(h,s,i, r, g, b);
			// printf("r,g,b: %d, %d, %d\n", r, g, b);
			tgt.setPixel(x,y,RED,checkValue(r));
			tgt.setPixel(x,y,GREEN,checkValue(g));
			tgt.setPixel(x,y,BLUE, checkValue(b));
		}
	}
}

// tgt image holds gradient value, dir image holds directional value
void utility::sobel3(image &src, image &tgt, image &dir){
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	dir.resize(src.getNumberOfRows(), src.getNumberOfColumns());

	int sobel_x[3][3] = {
		{-1, 0, 1},
		{-2, 0, 2},
		{-1, 0, 1}
	};
	int sobel_y[3][3] = {
		{1, 2, 1},
		{0, 0, 0},
		{-1, -2, -1}
	};
	for(int x=0; x < src.getNumberOfRows(); x++){
		for(int y=0; y < src.getNumberOfColumns(); y++){
			if(x==0 || y==0 || x==src.getNumberOfRows()-1|| y==src.getNumberOfColumns()-1){
				tgt.setPixel(x,y,0);
				dir.setPixel(x,y,tan(M_PI/4));
				continue;
			}
			int window[3][3]={
				{src.getPixel(x-1,y-1), src.getPixel(x-1,y), src.getPixel(x-1,y+1)},
				{src.getPixel(x,y-1), src.getPixel(x,y), src.getPixel(x,y+1)},
				{src.getPixel(x+1,y-1), src.getPixel(x+1,y), src.getPixel(x+1,y+1)}
			};
			// compute dx
			float dx = 0.0;
			for(int i=0; i<3; i++){
				for(int j=0; j<3; j++){
					dx += window[i][j]*sobel_x[i][j];
				}
			}
			dx = dx/8;
			// compute dy
			float dy = 0.0;
			for(int i=0; i<3; i++){
				for(int j=0; j<3; j++){
					dy += window[i][j]*sobel_y[i][j];
				}
			}
			dy = dy/8;
			// compute gradient
			int gradient = (int)floor(sqrt(dx*dx+dy*dy));
			// compute direction
			int theta = (int)floor(atan(dx/dy)*180/M_PI);

			tgt.setPixel(x,y,checkValue(gradient));
			dir.setPixel(x,y,theta);
		}
	}

	image copy;
	copy.copyImage(tgt);
	histostretch(copy,tgt,0,255);
}

// tgt image holds gradient value, dir image holds directional value
void utility::sobel5(image &src, image &tgt, image &dir){
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	dir.resize(src.getNumberOfRows(), src.getNumberOfColumns());

	int sobel_x[5][5] = {
		{-5, -4, 0, 4, 5},
		{-8, -10, 0, 10, 8},
		{-10, -20, 0, 20, 10},
		{-8, -10, 0, 10, 8},
		{-5, -4, 0, 4, 5}
	};
	int sobel_y[5][5] = {
		{-5, -8, -10, -8, -5},
		{-4, -10, -20, -10, -4},
		{0, 0, 0, 0, 0},
		{4, 10, 20, 10, 4},
		{5, 8, 10, 8, 5}
	};
	for(int x=0; x < src.getNumberOfRows(); x++){
		for(int y=0; y < src.getNumberOfColumns(); y++){
			if(x==0 || x==1 || y==0 || y==1 || x==src.getNumberOfRows()-1|| x==src.getNumberOfRows()-2 || y==src.getNumberOfColumns()-1 || y==src.getNumberOfColumns()-2){
				tgt.setPixel(x,y,0);
				dir.setPixel(x,y,tan(M_PI/4));
				continue;
			}
			int window[5][5]={
				{src.getPixel(x-2,y-2), src.getPixel(x-2, y-1), src.getPixel(x-2, y), src.getPixel(x-2, y+1), src.getPixel(x-2, y+2)},
				{src.getPixel(x-1,y-2), src.getPixel(x-1, y-1), src.getPixel(x-1, y), src.getPixel(x-1, y+1), src.getPixel(x-1, y+2)},
				{src.getPixel(x,y-2), src.getPixel(x, y-1), src.getPixel(x, y), src.getPixel(x, y+1), src.getPixel(x, y+2)},
				{src.getPixel(x+1,y-2), src.getPixel(x+1, y-1), src.getPixel(x+1, y), src.getPixel(x+1, y+1), src.getPixel(x+1, y+2)},
				{src.getPixel(x+2,y-2), src.getPixel(x+2, y-1), src.getPixel(x+2, y), src.getPixel(x+2, y+1), src.getPixel(x+2, y+2)}
			};
			// compute dx
			float dx = 0.0;
			for(int i=0; i<5; i++){
				for(int j=0; j<5; j++){
					dx += window[i][j]*sobel_x[i][j];
				}
			}
			dx = dx/240;
			// compute dy
			float dy = 0.0;
			for(int i=0; i<5; i++){
				for(int j=0; j<5; j++){
					dy += window[i][j]*sobel_y[i][j];
				}
			}
			dy = dy/240;
			// compute gradient
			int gradient = (int)floor(sqrt(dx*dx+dy*dy));
			// compute direction
			int theta = (int)floor(atan(dx/dy)*180/M_PI);

			tgt.setPixel(x,y,gradient);
			dir.setPixel(x,y,theta);
		}
	}

	image copy;
	copy.copyImage(tgt);
	histostretch(copy,tgt,0,255);
}

// ver image is the vertical directional threshold, hor image is the horizontal directional threshold
void utility::binaryedge(image &src, image &tgt, int th, int tl, image &ver, image &hor){
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());	
	ver.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	hor.resize(src.getNumberOfRows(), src.getNumberOfColumns());

	image gra(src.getNumberOfRows(),src.getNumberOfColumns());
	image dir(src.getNumberOfRows(),src.getNumberOfColumns());

	sobel3(src, gra, dir);

	tgt.copyImage(gra);

	
	// double thresholding
	for(int i=0; i<gra.getNumberOfRows(); i++){
		for (int j=0; j<gra.getNumberOfColumns(); j++){
			if(gra.getPixel(i,j)>th){
				tgt.setPixel(i,j,255);
			}else if(gra.getPixel(i,j)<tl){
				tgt.setPixel(i,j,0);
			}
			else{
				tgt.setPixel(i,j,src.getPixel(i,j));
			}
		}
	}

	// vertical thresholding: angle is 90 or 270, +/- 25 degrees
	for(int i=0; i<dir.getNumberOfRows(); i++){
		for (int j=0; j<dir.getNumberOfColumns(); j++){
			if(dir.getPixel(i,j)>65 && dir.getPixel(i,j)<115){
				ver.setPixel(i,j,tgt.getPixel(i,j));
			}else if(dir.getPixel(i,j)>245 && dir.getPixel(i,j)<295){
				ver.setPixel(i,j,tgt.getPixel(i,j));
			}
			else{
				ver.setPixel(i,j,0);
			}
		}
	}

	// horizontal thresholding: angle is 0 or 180, +/- 25 degrees
	for(int i=0; i<dir.getNumberOfRows(); i++){
		for (int j=0; j<dir.getNumberOfColumns(); j++){
			if(dir.getPixel(i,j)>=0 && dir.getPixel(i,j)<25){
				hor.setPixel(i,j,tgt.getPixel(i,j));
			}else if(dir.getPixel(i,j)>335 && dir.getPixel(i,j)<=360){
				hor.setPixel(i,j,tgt.getPixel(i,j));
			}else if(dir.getPixel(i,j)>155 && dir.getPixel(i,j)<205){
				hor.setPixel(i,j,tgt.getPixel(i,j));
			}
			else{
				hor.setPixel(i,j,0);
			}
		}
	}
}

void utility::sobelcv(image &src, image &tgt, int size){
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	image gra_x, gra_y;
	gra_x.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	gra_y.resize(src.getNumberOfRows(), src.getNumberOfColumns());

}

void utility::gausobel(image &src, image &tgt, float sigma){
	image gau, dir;
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	gau.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	dir.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	int range = (int)floor(3*sigma);

	float g_mat[6*range+1][6*range+1];
	// create gaussian matrix
	for(int i = -1*range; i<= range; i++){
		for(int j = -1*range; j<= range; j++){
			float g = (1/(2*M_PI*sigma*sigma))*exp((-1/(2*sigma*sigma))*(i*i+j*j));
			g_mat[i+range][j+range] = g;
		}
	}

	for(int x=0; x<src.getNumberOfRows(); x++){
		for(int y=0; y<src.getNumberOfColumns(); y++){
			// compute value with gaussian matrix
			float sum = 0.0;
			for(int i = -1*range; i<= range; i++){
				for(int j = -1*range; j<= range; j++){
					if(x+i<0 || x+i>src.getNumberOfRows()-1 || y+i<0 || y+i>src.getNumberOfColumns()-1){
						continue;
					}
					int value = src.getPixel(x+i, y+j);
					float f = value * g_mat[i+range][j+range];
					sum += f;
				}
			}
			gau.setPixel(x,y,(int)floor(sum));
		}
	}
	sobel5(gau, tgt, dir);
}