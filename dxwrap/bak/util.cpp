/******************************************************************************
*	filename:	utility.h
*	created:	7/11/2010
*	author:		Loser
*		
*	purpose:	工具函数集.
******************************************************************************/

#include <windows.h>

#define MLT_DLL_EXPORT

#include "../../../core/include/core.h"

#include "../include/util.h"

namespace mlt
{
	//-------------------------------------------------------------------------
	// 作用: 将源图像矩形rcSrc绘制到目的图像矩形rcDest的(X,Y)点,此函数
	//		会修改源图像矩形和目的图像矩形的有效区域.
	// 目的: 由于UpdateSurface限制,校验源图像rcSrc矩形,得到要更新的目标
	//		rcDest矩形.
	//
	void MDxUtil::CheckOutUpdateRect( RECT& rcDest, int X, int Y, RECT& rcSrc )
	{
		///////////////////////////////////////////////////////////////////////
		// 校验源矩形

		// X < 0 or Y < 0
		if ( X < 0 ) { rcSrc.left -= X; X = 0;	}
		if ( Y < 0 ) { rcSrc.top -= Y; Y = 0;	}

		// right - left > width or bottom - top > height
		rcSrc.right		= min( rcDest.right - rcDest.left - X + rcSrc.left, rcSrc.right );
		rcSrc.bottom	= min( rcDest.bottom - rcDest.top - Y + rcSrc.top, rcSrc.bottom );

		///////////////////////////////////////////////////////////////////
		// 返回目标矩形坐标
		rcDest.left		= X;
		rcDest.top		= Y;
		rcDest.right	= X + rcSrc.right - rcSrc.left;
		rcDest.bottom	= Y + rcSrc.bottom - rcSrc.top;
	}

	//-------------------------------------------------------------------------
	// 主要用于函数LoadSurfaceFromSurface(...)的矩形参数校验; 由于width和
	// height参数的限制,源矩形大小不能拉伸出矩形(0,0,width,height)的外面.
	void MDxUtil::CheckOutStretchRect( RECT& rcDest, int width, int height, RECT& rcSrc )
	{
		RECT destBound	= { 0, 0, width, height };
		RECT oldRect	= rcDest;

		float fScaleW	= (float)( rcSrc.right - rcSrc.left ) / (float)( rcDest.right - rcDest.left );
		float fScaleH	= (float)( rcSrc.bottom - rcSrc.top ) / (float)( rcDest.bottom - rcDest.top );

		MDxUtil::CheckOutUpdateRect( destBound, rcDest.left, rcDest.top, rcDest );

		rcSrc.left		= (long)(( rcDest.left - oldRect.left ) * fScaleW );
		rcSrc.right		= (long)(( rcDest.right - oldRect.left ) * fScaleW );
		rcSrc.top		= (long)(( rcDest.top - oldRect.top ) * fScaleH );
		rcSrc.bottom	= (long)(( rcDest.bottom - oldRect.top ) * fScaleH );

		rcDest = destBound;
	}

	//-------------------------------------------------------------------------
	//
}










