/******************************************************************************
*	filename:	texture.h
*	created:	8/1/2010
*	author:		Loser
*
*	purpose:	����
******************************************************************************/

#ifndef __MLT_DX_TEXTURE_H__
#define __MLT_DX_TEXTURE_H__

namespace mlt
{
	/**************************************************************************
	*	������
	**************************************************************************/

	DLL_CLASS MTexture
	{
	public:
		// ����/��������
		MTexture(MDirect3D* pD3D = MDirect3D::GetInst());
		virtual ~MTexture();

		bool Create(UINT nWidth, UINT nHeight, D3DPOOL pool = D3DPOOL_MANAGED, D3DFORMAT format = D3DFMT_A8R8G8B8);
		bool Create(LPCSTR pFilename, DWORD dwColorKey = 0xFF000000);
		void Release();

		bool Load(const RECT* pDestRect, LPCVOID pData, UINT uiSrcPitch, const PRECT pSrcRect, DWORD dwColorKey = 0xFF000000);
		bool Copy(const RECT* pDestRect, MTexture* pSrcTex, const PRECT pSrcRect, DWORD dwColorKey = 0xFF000000);

		bool Fill(const RECT* pRect, DWORD dwColor);
		void SetPixel(int X, int Y, DWORD dwColor);
		DWORD GetPixel(int X, int Y);
		
	public:
		// ȡ�������С
		inline int GetWidth() { return m_iWidth; }
		inline int GetHeight() { return m_iHeight; }

		// 
		inline PDIRECT3DTEXTURE9 GetITexture() { return m_pITexture; }
		inline const D3DSURFACE_DESC& GetDesc() { return m_surfaceDesc; }
		
		PDIRECT3DSURFACE9 GetISurface(UINT level = 0);

	protected:
		// D3D����ָ��
		MDirect3D*	m_pD3D;

		// ����
		PDIRECT3DTEXTURE9	m_pITexture;
		// ҳ��
		PDIRECT3DSURFACE9	m_pISurface;	
		// ����
		D3DSURFACE_DESC		m_surfaceDesc;

		// ����(������)��,��
		int		m_iWidth, m_iHeight;		
	};
}

#endif // __MLT_DX_TEXTURE_H__



