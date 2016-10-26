/******************************************************************************
*	filename:	gdifont.cpp
*	created:	8/26/2010
*	author:		Loser
*
*	purpose:	GDI Font.
******************************************************************************/

#include <stdio.h>
#include <windows.h>

#define MLT_DLL_EXPORT

#include "../../../core/include/core.h"

#include "../include/gdifont.h"

namespace mlt
{
	//-------------------------------------------------------------------------
	//
	MGdiFont::MGdiFont()
	{
		m_hFont = NULL;

		memset( m_szFace, 0, sizeof( m_szFace ));

		m_iSize			= 16;
		m_iWeight		= FW_NORMAL;
		m_boItalic		= false;
		m_boUnderline	= false;
		m_boStrikeOut	= false;

		m_dwColor = 0xFFFFFFFF;
	}

	//-------------------------------------------------------------------------
	//
	MGdiFont::~MGdiFont()
	{
		Release();
	}

	//-------------------------------------------------------------------------
	//
	bool MGdiFont::Create( int iSize, int iAngle, int iWeight,
						   bool boItalic, bool boUnderline, bool boStrikeOut,
						   DWORD dwCharSet, LPCSTR lpszFace )
	{
		m_hFont = ::CreateFont( iSize, 0, iAngle, iAngle, iWeight, boItalic, boUnderline, boStrikeOut, dwCharSet, 
								OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_ROMAN,
								lpszFace );
		if ( m_hFont )
		{
			_snprintf( m_szFace, sizeof( m_szFace ), lpszFace );

			m_iSize			= iSize;
			m_iWeight		= iWeight;
			m_boItalic		= boItalic;
			m_boUnderline	= boUnderline;
			m_boStrikeOut	= boStrikeOut;
		}

		return NULL != m_hFont;
	}

	//-------------------------------------------------------------------------
	//
	void MGdiFont::Release()
	{
		if ( m_hFont )
		{
			DeleteObject( m_hFont );
			m_hFont = NULL;
		}

		memset( m_szFace, 0, sizeof( m_szFace ));

		m_iSize			= 16;
		m_iWeight		= FW_NORMAL;
		m_boItalic		= false;
		m_boUnderline	= false;
		m_boStrikeOut	= false;

		m_dwColor = 0;
	}

	//-------------------------------------------------------------------------
	//
}
























