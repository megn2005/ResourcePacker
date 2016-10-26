/******************************************************************************
*	filename:	gdicanvas.h
*	created:	8/18/2010
*	author:		Loser
*
*	purpose:	
******************************************************************************/

#ifndef __MLT_GDICANVAS_H__
#define __MLT_GDICANVAS_H__

namespace mlt
{
	//=========================================================================
	//
	//=========================================================================

	DLL_CLASS MGdiCanvas : public MTexture
	{
	public:
		MGdiCanvas( MDirect3D* pD3D = MDirect3D::GetInst() );
		virtual ~MGdiCanvas();

		//
		bool Create( UINT uiWidth, UINT uiHeight, D3DPOOL pool = D3DPOOL_MANAGED, D3DFORMAT format = D3DFMT_A8R8G8B8 );
		void Release();

		// ��DC�����ã���������
		inline int GetBkMode() { return m_iBkMode; }
		inline void SetBkMode( int iMode ) { m_iBkMode = iMode; }

		inline COLORREF GetBkColor() { return m_dwBkColor; }
		inline void SetBkColor( COLORREF color ) { m_dwBkColor = color; }

		// �ı�
		LPCSTR TextPos( MGdiFont* pFont, LPCSTR pString, int width );
		int TextWidth( MGdiFont* pFont, LPCSTR pString, int nCount );

		bool TextOut( MGdiFont* pFont, int X, int Y, LPCSTR pString, int iCount );
		int DrawText( MGdiFont* pFont, LPCSTR pString, int iCount, PRECT pRect, UINT uiFormat = DT_LEFT | DT_TOP );

		// ����
		bool DrawLines( MGdiPen* pPen, MPoint<int> szPoint[], int iCount );  

	private:
		// DC����
		int			m_iBkMode;		// ����ģʽ(TRANSPARENT = 1, OPAQUE = 2)
		COLORREF	m_dwBkColor;	// ����ɫ
	};
}

#endif // __MLT_GDICANVAS_H__









