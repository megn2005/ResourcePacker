/******************************************************************************
*	filename:	utility.h
*	created:	7/11/2010
*	author:		Loser
*		
*	purpose:	���ߺ�����.
******************************************************************************/

#include <windows.h>

#define MLT_DLL_EXPORT

#include "../../../core/include/core.h"

#include "../include/util.h"

namespace mlt
{
	//-------------------------------------------------------------------------
	// ����: ��Դͼ�����rcSrc���Ƶ�Ŀ��ͼ�����rcDest��(X,Y)��,�˺���
	//		���޸�Դͼ����κ�Ŀ��ͼ����ε���Ч����.
	// Ŀ��: ����UpdateSurface����,У��Դͼ��rcSrc����,�õ�Ҫ���µ�Ŀ��
	//		rcDest����.
	//
	void MDxUtil::CheckOutUpdateRect( RECT& rcDest, int X, int Y, RECT& rcSrc )
	{
		///////////////////////////////////////////////////////////////////////
		// У��Դ����

		// X < 0 or Y < 0
		if ( X < 0 ) { rcSrc.left -= X; X = 0;	}
		if ( Y < 0 ) { rcSrc.top -= Y; Y = 0;	}

		// right - left > width or bottom - top > height
		rcSrc.right		= min( rcDest.right - rcDest.left - X + rcSrc.left, rcSrc.right );
		rcSrc.bottom	= min( rcDest.bottom - rcDest.top - Y + rcSrc.top, rcSrc.bottom );

		///////////////////////////////////////////////////////////////////
		// ����Ŀ���������
		rcDest.left		= X;
		rcDest.top		= Y;
		rcDest.right	= X + rcSrc.right - rcSrc.left;
		rcDest.bottom	= Y + rcSrc.bottom - rcSrc.top;
	}

	//-------------------------------------------------------------------------
	// ��Ҫ���ں���LoadSurfaceFromSurface(...)�ľ��β���У��; ����width��
	// height����������,Դ���δ�С�������������(0,0,width,height)������.
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










