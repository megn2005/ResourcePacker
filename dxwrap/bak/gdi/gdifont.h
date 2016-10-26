/******************************************************************************
*	filename:	gdifont.h
*	created:	8/26/2010
*	author:		Loser
*
*	purpose:	GDI Font.
******************************************************************************/

#ifndef __MLT_GDIFONT_H__
#define __MLT_GDIFONT_H__

namespace mlt
{
	#define	MAX_FACENAME_LEN	16

	//=========================================================================
	//
	//=========================================================================

	DLL_CLASS MGdiFont
	{
	public:
		MGdiFont();
		virtual ~MGdiFont();

		//
		bool Create( int iSize, int iAngle = 0, int iWeight = FW_NORMAL,
					 bool boItalic = false, bool boUnderline = false, bool boStrikeOut = false,
					 DWORD dwCharSet = GB2312_CHARSET, LPCSTR lpszFace = "宋体" );
		void Release();

		//
		inline HFONT GetHandle() { return m_hFont; }

		inline char* GetFace() { return m_szFace; }
		inline int GetSize() { return m_iSize; }
		inline int GetWeight() { return m_iWeight; }
		
		inline bool IsItalic() { return m_boItalic; }
		inline bool IsUnderline() { return m_boUnderline; }
		inline bool IsStrikeOut() { return m_boStrikeOut; }

	public:
		inline DWORD GetColor() { return m_dwColor; }
		inline void SetColor( DWORD dwColor ) { m_dwColor = dwColor; }

	private:
		HFONT	m_hFont;					// 句柄
		
		char	m_szFace[MAX_FACENAME_LEN];	// 类型
		int		m_iSize;					// 大小
		int		m_iWeight;					// 粗细
		bool	m_boItalic;					// 斜体
		bool	m_boUnderline;				// 下划线
		bool	m_boStrikeOut;				// 删除线

	private:
		DWORD	m_dwColor;		// 颜色
	};
}

#endif	// __MLT_GDIFONT_H__





