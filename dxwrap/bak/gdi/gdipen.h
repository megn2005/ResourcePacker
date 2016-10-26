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
		// ����/��������
		MGdiPen();
		virtual ~MGdiPen();

		// ���Է��֣�����ɫֵ����ֽ�Ϊ����ʱ������ʵ������Ч��
		bool Create(int iStyle = PS_SOLID, int iWidth = 0, DWORD dwColor = 0xFFFFFFFF);
		void Release();

		//
		inline HPEN GetHandle() { return m_hPen; }

		inline int GetStyle() { return m_iStyle; }
		inline int GetWidth() { return m_iWidth; }
		inline DWORD GetColor() { return m_dwColor; }

	private:
		// ���ʾ��
		HPEN	m_hPen;		
		
		// ������
		int		m_iStyle;	
		// �߿��
		int		m_iWidth;	
		// ����ɫ
		DWORD	m_dwColor;	
	};
}

#endif // __MLT_GDIPEN_H__