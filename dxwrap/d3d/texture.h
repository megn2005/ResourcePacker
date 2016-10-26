/******************************************************************************
*	�ļ���	:	texture.h
*	��������	:	7/10/2016
*	����		:	Loser
*	����		:	
******************************************************************************/

#ifndef __MLT_D3D_TEXTURE_H__
#define __MLT_D3D_TEXTURE_H__

namespace mlt
{
	/**************************************************************************
	* ������
	**************************************************************************/

	class MD3DTexture : public ID3DTexture
	{
		friend class MD3DDevice;

	public:
		// ��ȡҳ�����
		virtual LPDIRECT3DSURFACE9 GetSurface(UINT uLevel);
		// ��ȡ������Ϣ
		virtual BOOL GetDesc(UINT uLevel, D3DSURFACE_DESC* pDesc);

	public:
		// ��ȡ��ɫ��
		virtual PPALETTEENTRY GetPalette() { return m_pPalette; }

		// �������ҳ������
		virtual BOOL Clear(UINT uLevel, const RECT* pRect = NULL);

	public:
		// ����ҳ������
		//virtual BOOL LoadSurfaceFromFile(UINT uLevel, LPCTSTR pFile, //D3DXIMAGE_INFO* pImgInfo = NULL, 
		//						const RECT* pToRect = NULL, const RECT* pFromRect = NULL,
		//						D3DCOLOR dwColorKey = 0xFF000000, DWORD dwFilter = D3DX_DEFAULT);
		// ����ҳ������
		virtual BOOL LoadSurfaceFromMemory(UINT uLevel, LPCVOID pMemory, D3DFORMAT dwFormat, 
								const RECT* pRect, UINT uPitch, const PALETTEENTRY* pPalette = NULL, 
								const RECT* pStretchRect = NULL, D3DCOLOR dwColorKey = 0xFF000000, 
								DWORD dwFilter = D3DX_DEFAULT);
		// ����ҳ������
		virtual BOOL LoadSurfaceFromSurface(UINT uLevel, SP_ID3DTEXTURE spITex, 
								const RECT* pToRect = NULL, const RECT* pFromRect = NULL, 
								D3DCOLOR dwColorKey = 0xFF000000, DWORD dwFilter = D3DX_DEFAULT);
		
	protected:
		// ���캯��
		MD3DTexture(LPDIRECT3DTEXTURE9 pD3DTex9);

	private:
		// У���������
		bool checkUpdateRect(UINT uLevel, long& X, long& Y, RECT& rcSrc);
		bool checkStretchRect(UINT uLevel, RECT& rcDest, RECT& rcSrc);

	private:
		// ��ɫ��
		PPALETTEENTRY	m_pPalette;
	};
}


#endif // __MLT_D3D_TEXTURE_H__