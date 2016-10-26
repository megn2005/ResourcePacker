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
					 DWORD dwCharSet = GB2312_CHARSET, LPCSTR lpszFace = "����" );
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
		HFONT	m_hFont;					// ���
		
		char	m_szFace[MAX_FACENAME_LEN];	// ����
		int		m_iSize;					// ��С
		int		m_iWeight;					// ��ϸ
		bool	m_boItalic;					// б��
		bool	m_boUnderline;				// �»���
		bool	m_boStrikeOut;				// ɾ����

	private:
		DWORD	m_dwColor;		// ��ɫ
	};
}

#endif	// __MLT_GDIFONT_H__





