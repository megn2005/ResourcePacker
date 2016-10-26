/******************************************************************************
*	文件名	:	font.h
*	创建日期	:	8/18/2010
*	作者		:	Loser
*	描述		:	
******************************************************************************/

#ifndef __MLT_D3D_FONT_H__
#define __MLT_D3D_FONT_H__

namespace mlt
{
	/**************************************************************************
	* 类声明
	**************************************************************************/

	class MD3DFont : public ID3DFont
	{
		friend class MD3DDevice;

	public:
		// 绘制文本
		virtual int DrawText(LPRECT pRect, LPCTSTR pString, INT nCount = -1, 
							DWORD dwFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE, 
							D3DCOLOR dwColor = 0xFFFFFFFF, ID3DSprite* pISprite = NULL);

	protected:
		// 构造函数
		MD3DFont(LPD3DXFONT pD3DXFont);

	};
}


#endif // __MLT_D3D_FONT_H__
