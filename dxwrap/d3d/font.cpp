/******************************************************************************
*	�ļ���	:	font.cpp
*	��������	:	8/18/2010
*	����		:	Loser
*	����		:	
******************************************************************************/

#include "../dxwrap.h"
#include "font.h"
#include "sprite.h"

namespace mlt
{
	//-------------------------------------------------------------------------
	// �����ı�
	//-------------------------------------------------------------------------
	int MD3DFont::DrawText(LPRECT pRect, LPCTSTR pString, INT nCount /*= -1*/, 
						DWORD dwFormat /*= DT_LEFT | DT_VCENTER | DT_SINGLELINE*/, 
						D3DCOLOR dwColor /*= 0xFFFFFFFF*/, ID3DSprite* pISprite /*= NULL*/)
	{
		LPD3DXFONT pD3DXFont = static_cast<LPD3DXFONT>(m_pIUnknown);
		if (!pD3DXFont) return 0;

		//
		return pD3DXFont->DrawText(pISprite ? 
					dynamic_cast<LPD3DXSPRITE>(pISprite->GetInterface()) : NULL, 
					pString, nCount, pRect, dwFormat, dwColor);
	}


	//-------------------------------------------------------------------------
	// ���캯��
	//-------------------------------------------------------------------------
	MD3DFont::MD3DFont(LPD3DXFONT pD3DXFont) 
		: ID3DFont(pD3DXFont)
	{
	}

}


