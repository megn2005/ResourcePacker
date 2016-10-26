/******************************************************************************
*	filename:	gdicanvas.cpp
*	created:	8/18/2010
*	author:		Loser
*
*	purpose:	
******************************************************************************/

#include <map>
using namespace std;

#include <d3d9.h>
#include <d3dx9.h>

#define MLT_DLL_EXPORT

#include "../../../core/include/core.h"
#include "../../../core/include/point.h"

#include "../include/direct3d.h"
#include "../include/texture.h"

#include "../include/gdifont.h"
#include "../include/gdipen.h"
#include "../include/gdicanvas.h"

namespace mlt
{
	//-------------------------------------------------------------------------
	//
	MGdiCanvas::MGdiCanvas( MDirect3D* pD3D ) : MTexture( pD3D )
	{
		m_iBkMode	= TRANSPARENT;
		m_dwBkColor	= 0;
	}

	//-------------------------------------------------------------------------
	//
	MGdiCanvas::~MGdiCanvas()
	{
		Release();
	}

	//-------------------------------------------------------------------------
	//
	bool MGdiCanvas::Create( UINT uiWidth, UINT uiHeight, D3DPOOL pool, D3DFORMAT format )
	{
		return MTexture::Create( uiWidth, uiHeight, pool, format );
	}

	//-------------------------------------------------------------------------
	//
	void MGdiCanvas::Release()
	{
		m_iBkMode	= TRANSPARENT;
		m_dwBkColor	= 0;

		MTexture::Release();
	}

	//-------------------------------------------------------------------------
	//
	LPCSTR MGdiCanvas::TextPos( MGdiFont* pFont, LPCSTR pString, int width )
	{
		LPCSTR pCh = pString;

		if ( pString )
		{
			HDC hdc = NULL;
			PDIRECT3DSURFACE9 pISurface = GetISurface();

			if ( pISurface && SUCCEEDED( pISurface->GetDC( &hdc )))
			{
				int iLen = 0;
				SIZE sz = { 0, 0 };
				HFONT hOldFont = NULL;

				if ( pFont )
					hOldFont = ( HFONT )::SelectObject( hdc, pFont->GetHandle() );

				while ( *pCh )
				{
					if ( 0 < *pCh && *pCh < 128 )
						iLen = 1;
					else
						iLen = 2;

					::GetTextExtentPoint32( hdc, pCh, iLen, &sz );
					width -= sz.cx;

					if ( 0 <= width )
						pCh += iLen;
					else
						break;
				}

				if ( hOldFont )
					::SelectObject( hdc, hOldFont );

				pISurface->ReleaseDC( hdc );
			}
		}

		return pCh;
	}

	//-------------------------------------------------------------------------
	//
	int MGdiCanvas::TextWidth( MGdiFont* pFont, LPCSTR pString, int nCount )
	{
		SIZE sz = { 0, 0 };

		if ( pString )
		{
			HDC hdc = NULL;
			PDIRECT3DSURFACE9 pISurface = GetISurface();

			if ( pISurface && SUCCEEDED( pISurface->GetDC( &hdc )))
			{
				HFONT hOldFont = NULL;

				if ( pFont )
					hOldFont = ( HFONT )::SelectObject( hdc, pFont->GetHandle() );
 
				::GetTextExtentPoint32( hdc, pString, nCount, &sz );

				if ( hOldFont )
					::SelectObject( hdc, hOldFont );

				pISurface->ReleaseDC( hdc );
			}
		}

		return sz.cx;
	}

	//-------------------------------------------------------------------------
	//
	bool MGdiCanvas::TextOut( MGdiFont* pFont, int X, int Y, LPCSTR pString, int iCount )
	{
		bool result = false;

		if ( pString )
		{
			HDC hdc = NULL;
			PDIRECT3DSURFACE9 pISurface = GetISurface();

			if ( pISurface && SUCCEEDED( pISurface->GetDC( &hdc )))
			{
				HFONT hOldFont = NULL;

				if ( pFont )
				{
					hOldFont = ( HFONT )::SelectObject( hdc, pFont->GetHandle() );
					::SetTextColor( hdc, pFont->GetColor() );	// 文本颜色
				}

				::SetBkMode( hdc, m_iBkMode );		// 背景模式
				::SetBkColor( hdc, m_dwBkColor );	// 背景颜色

				result = FALSE != ::TextOut( hdc, X, Y, pString, iCount );
				
				if ( hOldFont )
					::SelectObject( hdc, hOldFont );

				pISurface->ReleaseDC( hdc );
			}
		}

		return result;
	}

	//-------------------------------------------------------------------------
	//
	int MGdiCanvas::DrawText( MGdiFont* pFont, LPCSTR pString, int iCount, PRECT pRect, UINT uiFormat )
	{
		int result = 0;
		
		if ( pString )
		{
			HDC hdc = NULL;
			PDIRECT3DSURFACE9 pISurface = GetISurface();

			if ( pISurface && SUCCEEDED( pISurface->GetDC( &hdc )))
			{
				HFONT hOldFont = NULL;

				if ( pFont )
				{
					hOldFont = ( HFONT )::SelectObject( hdc, pFont->GetHandle() );
					::SetTextColor( hdc, pFont->GetColor() );	// 文本颜色
				}
				
				::SetBkMode( hdc, m_iBkMode );		// 背景模式
				::SetBkColor( hdc, m_dwBkColor );	// 背景颜色

				result = ::DrawText( hdc, pString, iCount, pRect, uiFormat );

				if ( hOldFont )
					::SelectObject( hdc, hOldFont );

				pISurface->ReleaseDC( hdc );
			}
		}

		return result;
	}

	//-------------------------------------------------------------------------
	//
	bool MGdiCanvas::DrawLines( MGdiPen* pPen, MPoint<int> szPoint[], int iCount )
	{
		bool result = false;

		if ( 0 < iCount ) 
		{
			HDC hdc = NULL;
			PDIRECT3DSURFACE9 pISurface = GetISurface();

			if ( pISurface && SUCCEEDED( pISurface->GetDC( &hdc )))
			{
				HPEN hOldPen = NULL;

				if ( pPen )
					hOldPen = ( HPEN )::SelectObject( hdc, pPen->GetHandle() );

				if ( ::MoveToEx( hdc, szPoint[0].x, szPoint[0].y, NULL ))
					for ( int i = 1; i < iCount; i++ )
						::LineTo( hdc, szPoint[i].x, szPoint[i].y );

				if ( hOldPen )
					::SelectObject( hdc, hOldPen );

				result = SUCCEEDED( pISurface->ReleaseDC( hdc ));
			}
		}

		return result;
	}

	//-------------------------------------------------------------------------
	//
}





