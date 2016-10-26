/******************************************************************************
*	filename:	gdipen.cpp
*	created:	4/4/2012
*	author:		Loser
*		
*	purpose:
******************************************************************************/

#include <windows.h>

#define MLT_DLL_EXPORT

#include "../../../core/include/core.h"

#include "../include/gdipen.h"

namespace mlt
{
	//-------------------------------------------------------------------------
	//
	MGdiPen::MGdiPen()
	{
		m_hPen		= NULL;
		
		m_iStyle	= PS_SOLID;
		m_iWidth	= 0;
		m_dwColor	= 0xFFFFFFFF;
	}

	//-------------------------------------------------------------------------
	//
	MGdiPen::~MGdiPen()
	{
		Release();
	}

	//-------------------------------------------------------------------------
	//
	bool MGdiPen::Create( int iStyle, int iWidth, DWORD dwColor )
	{
		m_hPen = ::CreatePen( iStyle, iWidth, dwColor );
		if ( m_hPen )
		{
			m_iStyle	= iStyle;
			m_iWidth	= iWidth;
			m_dwColor	= dwColor;
		}

		return NULL != m_hPen;
	}

	//-------------------------------------------------------------------------
	//
	void MGdiPen::Release()
	{
		if ( m_hPen )
		{
			DeleteObject( m_hPen );
			m_hPen = NULL;
		}

		m_iStyle	= PS_SOLID;
		m_iWidth	= 0;
		m_dwColor	= 0xFFFFFFFF;
	}

	//-------------------------------------------------------------------------
	//

}