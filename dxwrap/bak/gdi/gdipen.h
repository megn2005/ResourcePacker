/******************************************************************************
*	filename:	gdipen.h
*	created:	4/4/2012
*	author:		Loser
*		
*	purpose:
******************************************************************************/

#ifndef __MLT_GDIPEN_H__
#define __MLT_GDIPEN_H__

namespace mlt
{
	//=========================================================================
	//
	//=========================================================================

	DLL_CLASS MGdiPen
	{
	public:
		// 构造/析构函数
		MGdiPen();
		virtual ~MGdiPen();

		// 测试发现，当颜色值最高字节为奇数时，才能实现虚线效果
		bool Create(int iStyle = PS_SOLID, int iWidth = 0, DWORD dwColor = 0xFFFFFFFF);
		void Release();

		//
		inline HPEN GetHandle() { return m_hPen; }

		inline int GetStyle() { return m_iStyle; }
		inline int GetWidth() { return m_iWidth; }
		inline DWORD GetColor() { return m_dwColor; }

	private:
		// 画笔句柄
		HPEN	m_hPen;		
		
		// 线类型
		int		m_iStyle;	
		// 线宽度
		int		m_iWidth;	
		// 线颜色
		DWORD	m_dwColor;	
	};
}

#endif // __MLT_GDIPEN_H__