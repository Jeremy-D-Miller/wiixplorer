/***************************************************************************
 * Copyright (C) 2010
 * Dimok
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you
 * must not claim that you wrote the original software. If you use
 * this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 *
 * TextureConverter.cpp
 *
 * for WiiXplorer 2010
 ***************************************************************************/
#include <gccore.h>
#include <malloc.h>
#include "TextureConverter.h"

static u16 avg(u16 w0, u16 w1, u16 c0, u16 c1)
{
	u16 a0, a1;
	u16 a, c;

	a0 = c0 >> 11;
	a1 = c1 >> 11;
	a = (w0*a0 + w1*a1) / (w0 + w1);
	c = a << 11;

	a0 = (c0 >> 5) & 63;
	a1 = (c1 >> 5) & 63;
	a = (w0*a0 + w1*a1) / (w0 + w1);
	c |= a << 5;

	a0 = c0 & 31;
	a1 = c1 & 31;
	a = (w0*a0 + w1*a1) / (w0 + w1);
	c |= a;

	return c;
}

bool I4ToGD(const u8 * buffer, u32 width, u32 height, gdImagePtr * im)
{
	u32 x, y;
	u32 x1, y1;
	u32 iv;

	if(!buffer)
        return false;

	*im = gdImageCreateTrueColor(width, height);
	if(*im == 0)
		return false;

    gdImageAlphaBlending(*im, 0);
    gdImageSaveAlpha(*im, 1);

	for(iv = 0, y1 = 0; y1 < height; y1 += 8)
	{
		for(x1 = 0; x1 < width; x1 += 8)
		{
			for(y = y1; y < (y1 + 8); y++)
			{
				for(x = x1; x < (x1 + 8); x += 2, iv++)
				{
					if((x >= width) || (y >= height))
						continue;

					u8 oldpixel = buffer[iv];
					u8 b = (oldpixel >> 4) * 255 / 15;
					u8 g = (oldpixel >> 4) * 255 / 15;
					u8 r = (oldpixel >> 4) * 255 / 15;
					u8 a = 0;

					gdImageSetPixel(*im, x, y, gdTrueColorAlpha(r, g, b, a));

					r = (oldpixel & 0xF) * 255 / 15;
					g = (oldpixel & 0xF) * 255 / 15;
					b = (oldpixel & 0xF) * 255 / 15;
					a = gdAlphaOpaque;

					gdImageSetPixel(*im, x+1, y, gdTrueColorAlpha(r, g, b, a));
				}
			}
		}
	}
	return true;
}

bool IA4ToGD(const u8 * buffer, u32 width, u32 height, gdImagePtr * im)
{
	u32 x, y;
	u32 x1, y1;
	u32 iv;

	if(!buffer)
        return false;

	*im = gdImageCreateTrueColor(width, height);
	if(*im == 0)
		return false;

    gdImageAlphaBlending(*im, 0);
    gdImageSaveAlpha(*im, 1);

	for(iv = 0, y1 = 0; y1 < height; y1 += 4)
	{
		for(x1 = 0; x1 < width; x1 += 8)
		{
			for(y = y1; y < (y1 + 4); y++)
			{
				for(x = x1; x < (x1 + 8); x++)
				{
					u8 oldpixel = *(u8*)(buffer + (iv++));
					oldpixel = ~oldpixel;

					if((x >= width) || (y >= height))
						continue;

					u8 r = ((oldpixel & 0xF) * 255) / 15;
					u8 g = ((oldpixel & 0xF) * 255) / 15;
					u8 b = ((oldpixel & 0xF) * 255) / 15;
					u8 a = ((oldpixel >> 4)  * 255) / 15;
                    a = 127-127*a/255;

					gdImageSetPixel(*im, x+1, y, gdTrueColorAlpha(r, g, b, a));
				}
			}
		}
	}
	return true;
}

bool I8ToGD(const u8 * buffer, u32 width, u32 height, gdImagePtr * im)
{
	u32 x, y;
	u32 x1, y1;
	u32 iv;

	if(!buffer)
        return false;

	*im = gdImageCreateTrueColor(width, height);
	if(*im == 0)
		return false;

    gdImageAlphaBlending(*im, 0);
    gdImageSaveAlpha(*im, 1);

	for(iv = 0, y1 = 0; y1 < height; y1 += 4)
	{
		for(x1 = 0; x1 < width; x1 += 8)
		{
			for(y = y1; y < (y1 + 4); y++)
			{
				for(x = x1; x < (x1 + 8); x++)
				{
					u8 pixel = *(u8*)(buffer + ((iv++) * 1));
					if((x >= width) || (y >= height))
						continue;

					u8 r = pixel;
					u8 g = pixel;
					u8 b = pixel;
					u8 a = gdAlphaOpaque;

					gdImageSetPixel(*im, x, y, gdTrueColorAlpha(r, g, b, a));
				}
			}
		}
	}

	return true;
}

bool IA8ToGD(const u8 * buffer, u32 width, u32 height, gdImagePtr * im)
{
	u32 x, y;
	u32 x1, y1;
	u32 iv;

	if(!buffer)
        return false;

	*im = gdImageCreateTrueColor(width, height);
	if(*im == 0)
		return false;

    gdImageAlphaBlending(*im, 0);
    gdImageSaveAlpha(*im, 1);

	for(iv = 0, y1 = 0; y1 < height; y1 += 4)
	{
		for(x1 = 0; x1 < width; x1 += 4)
		{
			for(y = y1; y < (y1 + 4); y++)
			{
				for(x = x1; x < (x1 + 4); x++)
				{
					u16 oldpixel = *(u16*)(buffer + ((iv++) * 2));

					if((x >= width) || (y >= height))
						continue;

					u8 r = oldpixel >> 8;
					u8 g = oldpixel >> 8;
					u8 b = oldpixel >> 8;
					u8 a = oldpixel & 0xFF;
                    a = 127-127*a/255;

					gdImageSetPixel(*im, x+1, y, gdTrueColorAlpha(r, g, b, a));
				}
			}
		}
	}
	return true;
}

bool CMPToGD(const u8* buffer, u32 width, u32 height, gdImagePtr * im)
{
	u32 x, y;
    u8 r, g, b, a;
    u16 raw;
    u16 c[4];
    int x0, x1, x2, y0, y1, y2, off;
    int ww = (-(-(width) & -(8)));
    int ix;
    u32 px;

	if(!buffer)
        return false;

	*im = gdImageCreateTrueColor(width, height);
	if(*im == 0)
		return false;

    gdImageAlphaBlending(*im, 0);
    gdImageSaveAlpha(*im, 1);

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			x0 = x & 3;
			x1 = (x >> 2) & 1;
			x2 = x >> 3;
			y0 = y & 3;
			y1 = (y >> 2) & 1;
			y2 = y >> 3;
			off = (8 * x1) + (16 * y1) + (32 * x2) + (4 * ww * y2);

			c[0] = *(u16*)(buffer + off);
			c[1] = *(u16*)(buffer + off + 2);
			if (c[0] > c[1]) {
				c[2] = avg(2, 1, c[0], c[1]);
				c[3] = avg(1, 2, c[0], c[1]);
			} else {
				c[2] = avg(1, 1, c[0], c[1]);
				c[3] = 0;
			}

			px = *(u32*)(buffer + off + 4);
			ix = x0 + (4 * y0);
			raw = c[(px >> (30 - (2 * ix))) & 3];

			r = (raw >> 8) & 0xf8;
			g = (raw >> 3) & 0xf8;
			b = (raw << 3) & 0xf8;
			a = gdAlphaOpaque;

            gdImageSetPixel(*im, x, y, gdTrueColorAlpha(r, g, b, a));
		}
	}

	return true;
}

bool RGB565ToGD(const u8* buffer, u32 width, u32 height, gdImagePtr * im)
{
	u32 x, y;
	u32 x1, y1;
	u32 iv;

	if(!buffer)
        return false;

	*im = gdImageCreateTrueColor(width, height);
	if(*im == 0)
		return false;

    gdImageAlphaBlending(*im, 0);
    gdImageSaveAlpha(*im, 1);

	for(iv = 0, y1 = 0; y1 < height; y1 += 4)
	{
		for(x1 = 0; x1 < width; x1 += 4)
		{
			for(y = y1; y < (y1 + 4); y++)
			{
				for(x = x1; x < (x1 + 4); x++)
				{
					if((x >= width) || (y >= height))
						continue;

					u16 pixel = *(u16*)(buffer + ((iv++) * 2));

					u8 r = ((pixel >> 11) & 0x1F) << 3;
					u8 g = ((pixel >> 5)  & 0x3F) << 2;
					u8 b = ((pixel >> 0)  & 0x1F) << 3;
					u8 a = gdAlphaOpaque;

                    gdImageSetPixel(*im, x, y, gdTrueColorAlpha(r, g, b, a));
				}
			}
		}
	}
	return true;
}

bool RGB565A3ToGD(const u8* buffer, u32 width, u32 height, gdImagePtr * im)
{
	u32 x, y;
	u32 x1, y1;
	u32 iv;

	if(!buffer)
        return false;

	*im = gdImageCreateTrueColor(width, height);
	if(*im == 0)
		return false;

    gdImageAlphaBlending(*im, 0);
    gdImageSaveAlpha(*im, 1);

	for(iv = 0, y1 = 0; y1 < height; y1 += 4)
	{
		for(x1 = 0; x1 < width; x1 += 4)
		{
			for(y = y1; y < (y1 + 4); y++)
			{
				for(x = x1; x < (x1 + 4); x++)
				{
					u16 pixel = *(u16*)(buffer + ((iv++) * 2));
					if((x >= width) || (y >= height))
						continue;

					if(pixel & (1 << 15))
					{
						// RGB5
						u8 r = (((pixel >> 10) & 0x1F) * 255) / 31;
						u8 g = (((pixel >> 5)  & 0x1F) * 255) / 31;
						u8 b = (((pixel >> 0)  & 0x1F) * 255) / 31;
						u8 a = gdAlphaOpaque;

                        gdImageSetPixel(*im, x, y, gdTrueColorAlpha(r, g, b, a));
					}
					else
					{
						// RGB4A3
						u8 r = (((pixel >> 12) & 0xF) * 255) / 15;
						u8 g = (((pixel >> 8)  & 0xF) * 255) / 15;
						u8 b = (((pixel >> 4)  & 0xF) * 255) / 15;
						u8 a = (((pixel >> 0)  & 0x7) * 64) / 7;
                        a = 127-127*a/255;

                        gdImageSetPixel(*im, x, y, gdTrueColorAlpha(r, g, b, a));
					}
				}
			}
		}
	}

	return true;
}

bool RGBA8ToGD(const u8* buffer, u32 width, u32 height, gdImagePtr * im)
{
	u32 x, y;
	u8 r, g, b, a;

	if(!buffer)
        return false;

	*im = gdImageCreateTrueColor(width, height);
	if(*im == 0)
		return false;

    gdImageAlphaBlending(*im, 0);
    gdImageSaveAlpha(*im, 1);

	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
		    u32 offset = ((((y >> 2) * (width >> 2) + (x >> 2)) << 5) + ((y & 3) << 2) + (x & 3)) << 1;
            a = *(buffer+offset);
            r = *(buffer+offset+1);
            g = *(buffer+offset+32);
            b = *(buffer+offset+33);

            a = 127-127*a/255;

            gdImageSetPixel(*im, x, y, gdTrueColorAlpha(r, g, b, a));
		}
	}

	return true;
}

u8 * GDImageToRGBA8(gdImagePtr gdImg)
{
	int width = gdImageSX(gdImg);
	int height = gdImageSY(gdImg);

    int newwidth = width;
    int newheight = height;

    if(newwidth % 4)
        newwidth -= newwidth % 4;

    if(newheight % 4)
        newheight -= newheight % 4;

    if(newwidth != width || newheight != height)
    {
        gdImagePtr dst = gdImageCreateTrueColor(newwidth, newheight);
        gdImageCopyResized(dst, gdImg, 0, 0, 0, 0, newwidth, newheight, width, height);

        gdImageDestroy(gdImg);
        gdImg = dst;

        width = gdImageSX(gdImg);
        height = gdImageSY(gdImg);
    }

    int len =  ((width+3)>>2)*((height+3)>>2)*32*2;
    if(len%32)
        len += (32-len%32);

    u8 * data = (u8 *) memalign (32, len);
    if(!data)
        return NULL;

	int x, y;

    for(y = 0; y < height; y++)
    {
        for(x = 0; x < width; x++)
        {
            u32 p = gdImageGetPixel(gdImg, x, y);

            u8 a = 254 - 2*((u8)gdImageAlpha(gdImg, p));
            if(a == 254) a++;
            u8 r = (u8)gdImageRed(gdImg, p);
            u8 g = (u8)gdImageGreen(gdImg, p);
            u8 b = (u8)gdImageBlue(gdImg, p);

            u32 offset = ((((y >> 2) * (width >> 2) + (x >> 2)) << 5) + ((y & 3) << 2) + (x & 3)) << 1;
            data[offset] = a;
            data[offset+1] = r;
            data[offset+32] = g;
            data[offset+33] = b;
        }
    }

    DCFlushRange(data, len);

    return data;
}