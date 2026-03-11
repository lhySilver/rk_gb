/*
 *  Copyright 2010-2011 ZXing authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "YUVVideoSource.h"
#include <zxing/common/IllegalArgumentException.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include "lodepng.h"
#include "jpgd.h"
//#define TRANS_TO_GRAY


using std::string;
using std::ostringstream;
using zxing::Ref;
using zxing::ArrayRef;
using zxing::LuminanceSource;

YUVVideoSource::YUVVideoSource(ArrayRef<char> image_, int width, int height)
    : Super(width, height), image(image_) {}


static inline int YUVToGray(int Y, int U, int V)
{
	int r, g, b;
	
	r = ((Y << 8) + ((V << 8) + (V << 5) + (V << 2))) >> 8;  
	g =  ((Y << 8) - ((U << 6) + (U << 5) + (U << 2)) - ((V << 7) + (V << 4) + (V << 2) + V)) >> 8;    
	b = ((Y << 8) + (U << 9) + (U << 3)) >> 8;  
	return (r*306 + g*601 + b*117) >> 10;
}

Ref<LuminanceSource> YUVVideoSource::create(char *pbuf, 
		int width, int height)
{
	zxing::ArrayRef<char> image;
	int i, j;
	int rgb_idx = 0;
	int nv_idx = 0;
	int offset = width * height;

	image = zxing::ArrayRef<char>(width * height);

#ifdef TRANS_TO_GRAY
	printf("gray\n");
	for (i = 0; i < height; ++i) {
		for (j = 0; j < width; ++j) {
			nv_idx = offset + i / 2  * width + j - (j & 1);
			image[rgb_idx] = YUVToGray(pbuf[rgb_idx], pbuf[nv_idx], pbuf[nv_idx + 1]);
			rgb_idx++;
		}
	}
#else
	memcpy(&image[0], pbuf, width * height);
#endif
  	return Ref<LuminanceSource>(new YUVVideoSource(image, width, height));
}

zxing::ArrayRef<char> YUVVideoSource::getRow(int y, zxing::ArrayRef<char> row) const {
  const char* pixelRow = &image[0] + y * getWidth();
  if (!row) {
    row = zxing::ArrayRef<char>(getWidth());
  }
  for (int x = 0; x < getWidth(); x++) {
    row[x] = *(pixelRow + x);
  }
  return row;
}

/** This is a more efficient implementation. */
zxing::ArrayRef<char> YUVVideoSource::getMatrix() const {
  return image;
}


