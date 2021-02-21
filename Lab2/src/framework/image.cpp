#include "image.h"


Image::Image() {
	width = 0; height = 0;
	pixels = NULL;
}

Image::Image(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new Color[width*height];
	memset(pixels, 0, width * height * sizeof(Color));
}

//copy constructor
Image::Image(const Image& c) {
	pixels = NULL;

	width = c.width;
	height = c.height;
	if(c.pixels)
	{
		pixels = new Color[width*height];
		memcpy(pixels, c.pixels, width*height*sizeof(Color));
	}
}

//assign operator
Image& Image::operator = (const Image& c)
{
	if(pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	if(c.pixels)
	{
		pixels = new Color[width*height*sizeof(Color)];
		memcpy(pixels, c.pixels, width*height*sizeof(Color));
	}
	return *this;
}

Image::~Image()
{
	if(pixels) 
		delete pixels;
}



//change image size (the old one will remain in the top-left corner)
void Image::resize(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for(unsigned int x = 0; x < min_width; ++x)
		for(unsigned int y = 0; y < min_height; ++y)
			new_pixels[ y * width + x ] = getPixel(x,y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

//change image size and scale the content
void Image::scale(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];

	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
			new_pixels[ y * width + x ] = getPixel((unsigned int)(this->width * (x / (float)width)), (unsigned int)(this->height * (y / (float)height)) );

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

Image Image::getArea(unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height)
{
	Image result(width, height);
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
		{
			if( (x + start_x) < this->width && (y + start_y) < this->height) 
				result.setPixel( x, y, getPixel(x + start_x,y + start_y) );
		}
	return result;
}

void Image::flipX()
{
	for(unsigned int x = 0; x < width * 0.5; ++x)
		for(unsigned int y = 0; y < height; ++y)
		{
			Color temp = getPixel(width - x - 1, y);
			setPixel( width - x - 1, y, getPixel(x,y));
			setPixel( x, y, temp );
		}
}

void Image::flipY()
{
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height * 0.5; ++y)
		{
			Color temp = getPixel(x, height - y - 1);
			setPixel( x, height - y - 1, getPixel(x,y) );
			setPixel( x, y, temp );
		}
}


//Loads an image from a TGA file
bool Image::loadTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char TGAcompare[12];
	unsigned char header[6];
	unsigned int bytesPerPixel;
	unsigned int imageSize;

	FILE * file = fopen(filename, "rb");
   	if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
		fread(header, 1, sizeof(header), file) != sizeof(header))
	{
		std::cerr << "File not found: " << filename << std::endl;
		if (file == NULL)
			return NULL;
		else
		{
			fclose(file);
			return NULL;
		}
	}

	TGAInfo* tgainfo = new TGAInfo;
    
	tgainfo->width = header[1] * 256 + header[0];
	tgainfo->height = header[3] * 256 + header[2];
    
	if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
	{
		std::cerr << "TGA file seems to have errors or it is compressed, only uncompressed TGAs supported" << std::endl;
		fclose(file);
		delete tgainfo;
		return NULL;
	}
    
	tgainfo->bpp = header[4];
	bytesPerPixel = tgainfo->bpp / 8;
	imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;
    
	tgainfo->data = new unsigned char[imageSize];
    
	if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
	{
		if (tgainfo->data != NULL)
			delete tgainfo->data;
            
		fclose(file);
		delete tgainfo;
		return false;
	}

	fclose(file);

	//save info in image
	if(pixels)
		delete pixels;

	width = tgainfo->width;
	height = tgainfo->height;
	pixels = new Color[width*height];

	//convert to float all pixels
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			unsigned int pos = y * width * bytesPerPixel + x * bytesPerPixel;
			this->setPixel(x , height - y - 1, Color( tgainfo->data[pos+2], tgainfo->data[pos+1], tgainfo->data[pos]) );
		}

	delete tgainfo->data;
	delete tgainfo;

	return true;
}

// Saves the image to a TGA file
bool Image::saveTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	FILE *file = fopen(filename, "wb");
	if ( file == NULL )
	{
		fclose(file);
		return false;
	}

	unsigned short header_short[3];
	header_short[0] = width;
	header_short[1] = height;
	unsigned char* header = (unsigned char*)header_short;
	header[4] = 24;
	header[5] = 0;

	//tgainfo->width = header[1] * 256 + header[0];
	//tgainfo->height = header[3] * 256 + header[2];

	fwrite(TGAheader, 1, sizeof(TGAheader), file);
	fwrite(header, 1, 6, file);

	//convert pixels to unsigned char
	unsigned char* bytes = new unsigned char[width*height*3];
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			Color c = pixels[(height-y-1)*width+x];
			unsigned int pos = (y*width+x)*3;
			bytes[pos+2] = c.r;
			bytes[pos+1] = c.g;
			bytes[pos] = c.b;
		}

	fwrite(bytes, 1, width*height*3, file);
	fclose(file);
	return true;
}

void Image::DDA(int x1, int y1, int x2, int y2, Color c)
{
	float d, x, y;
	float dx = (x2 - x1);
	float dy = (y2 - y1);
	if (fabs(dx) >= fabs(dy))
		d = fabs(dx);
	else
		d = fabs(dy);
	float vx = dx / d;
	float vy = dy / d;
	x = x1 + sgn(x1) * 0.5;
	y = y1 + sgn(y1) * 0.5;
	for (int i = 0; i <= d; i++)
	{
		setPixel(round(x), round(y), c);
		x = x + vx;
		y = y + vy;
	}
}

//function sgn returns -1 if the value is negative, otherwise +1
int Image::sgn(int x)
{
	if (x < 0)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}


void Image::BresenhamLine(int x0, int y0, int x1, int y1, Color c) {
	int dx, dy, d;
	
	dx = x1 - x0;
	dy = y1 - y0;

	int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;

	if (dx > 0) {
		dx1 = 1;
		dx2 = 1;
	}
	else {
		dx1 = -1;
		dx2 = -1;
	}
	 
	if (dy > 0) dy1 = 1; else dy1 = -1;


	int big = abs(dx);
	int small = abs(dy);

	if (big < small) {
		big = abs(dy);
		small = abs(dx);
		if (dy < 0) dy2 = -1; else if (dy > 0) dy2 = 1;
		dx2 = 0;
	}
	d = big / 2;
	for (int i = 0; i <= big; i++) {
		setPixel(x0, y0, c);
		d += small;
		if (d < big) {
			x0 += dx2;
			y0 += dy2;
		}
		else {
			d -= big;
			x0 += dx1;
			y0 += dy1;
		}
	}
}

void Image::BresenhamCircle(int x0, int y0, int radius, Color c, bool fill) {
	int x, y; int v;
	x = 0;
	y = radius;
	v = 1 - radius;
	EightOctantsCircleDraw(x0, y0, x, y, c, fill);
	while (y > x) {
		if (v < 0) {
			v = v + 2 * x + 3;
			x++;
		}
		else {
			v = v + 2 * (x - y) + 5;
			x++;
			y--;
		}
		EightOctantsCircleDraw(x0, y0, x, y, c, fill);
	}
}

//Function to draw all 8 octants of the circle without changing the function of the circle
void Image::EightOctantsCircleDraw(int x0, int y0, int x, int y, Color c, bool fill) {
	if (fill) {
		BresenhamLine(-x + x0, y + y0, x + x0, y + y0, c);   //line from 8th to 1st octant
		BresenhamLine(-y + x0, x + y0, y + x0, x + y0, c);   //line from 7th to 2nd octant
		BresenhamLine(-y + x0, -x + y0, y + x0, -x + y0, c); //line from 6th to 3rd octant
		BresenhamLine(-x + x0, -y + y0, x + x0, -y + y0, c);   //line from 5th to 4th octant
	}
	else {
		setPixel(x + x0, y + y0, c);   //1st octant     
		setPixel(y + x0, x + y0, c);   //2nd octant
		setPixel(y + x0, -x + y0, c);  //3rd octant
		setPixel(x + x0, -y + y0, c);  //4th octant
		setPixel(-x + x0, -y + y0, c); //5th octant
		setPixel(-y + x0, -x + y0, c); //6th octant
		setPixel(-y + x0, x + y0, c);  //7th octant
		setPixel(-x + x0, y + y0, c);  //8th octant
	}
} 

void Image::drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color c, bool fill)
{
	if (!fill) {
		BresenhamLine(x0, y0, x1, y1, c);
		BresenhamLine(x1, y1, x2, y2, c);
		BresenhamLine(x2, y2, x0, y0, c);
	}
	else {
	}
}

void Image::BresenhamwithTable(int x0, int y0, int x1, int y1, std::vector<sCelda>& table) {
	//evaluar si el punt est� tocant la l�nia del triangle
	int dx = x1 - x0;
	int dy = y1 - y0;
	int x; int y;
	int L = (-x - x0) * dy + (y - y0) * dx;
	for (int y = 0; y < this->height; y++) {
		for (int x = 0; x < this->width; x++) {
			{
				if (L == 0) { // troba la l�nia
					if (x < table[y].minx) { table[y].minx = x; }
					if (x > table[y].maxx) { table[y].maxx = x; }
				}
			}
		}
	}
}
#ifndef IGNORE_LAMBDAS

//you can apply and algorithm for two images and store the result in the first one
//forEachPixel( img, img2, [](Color a, Color b) { return a + b; } );
template <typename F>
void forEachPixel(Image& img, const Image& img2, F f) {
	for(unsigned int pos = 0; pos < img.width * img.height; ++pos)
		img.pixels[pos] = f( img.pixels[pos], img2.pixels[pos] );
}

#endif