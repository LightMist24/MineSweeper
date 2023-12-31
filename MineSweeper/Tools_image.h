#pragma once
#include <conio.h>
#include <easyx.h>

//色码转还器
//https://www.rapidtables.org/zh-CN/convert/color/index.html
//这里的RGB，不如此。

//图片背景透明
void transparentimg(int picture_x, int picture_y, IMAGE* picture)
{
	//变量初始化
	DWORD* dst = GetImageBuffer();	//GetImageBuffer()函数用于获取绘图设备的显存，EasyX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture);	//获取图像的显存指针
	int picture_width = picture->getwidth();	//获取picture的宽度，EasyX自带
	int picture_height = picture->getheight();	//获取picture的高度，EasyX自带
	int graphWidth = getwidth();	//获取绘图区的宽度，EasyX自带
	int graphHeight = getheight();	//获取绘图区的高度，EasyX自带
	int dstX = 0;	//在显存里像素的角标

	//实现透明贴图 公式：Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率运算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width;	//在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24);	//0xAArrggbb, AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16);	//获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);	//获取RGB里的G
			int sb = src[srcX] & 0xff;	//获取RGB里的B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth;	//在显存里的显存的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)	//公式：Cp=αp*FP+(1-αp)*BP ； αp=sa/255 ， FP=sr ， BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)	//αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);			//αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}

//图片背景(自定义)
void setimgbkcolor(int picture_x, int picture_y, IMAGE* picture, int r, int g, int b)
{
	//变量初始化
	DWORD* dst = GetImageBuffer();	//GetImageBuffer()函数用于获取绘图设备的显存，EasyX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture);	//获取图像的显存指针
	int picture_width = picture->getwidth();	//获取picture的宽度，EasyX自带
	int picture_height = picture->getheight();	//获取picture的高度，EasyX自带
	int graphWidth = getwidth();	//获取绘图区的宽度，EasyX自带
	int graphHeight = getheight();	//获取绘图区的高度，EasyX自带
	int dstX = 0;	//在显存里像素的角标

	//实现透明贴图 公式：Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率运算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width;	//在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24);	//0xAArrggbb, AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16);	//获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);	//获取RGB里的G
			int sb = src[srcX] & 0xff;	//获取RGB里的B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth;	//在显存里的显存的角标
				int dr = ((dst[dstX] & 0xff0000) >> r);		//R设置
				int dg = ((dst[dstX] & 0xff00) >> g);		//G设置
				int db = ((dst[dstX] & 0xff) >> b);			//B设置
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)	//公式：Cp=αp*FP+(1-αp)*BP ； αp=sa/255 ， FP=sr ， BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)	//αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);			//αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}