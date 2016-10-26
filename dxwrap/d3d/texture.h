/******************************************************************************
*	文件名	:	texture.h
*	创建日期	:	7/10/2016
*	作者		:	Loser
*	描述		:	
******************************************************************************/

#ifndef __MLT_D3D_TEXTURE_H__
#define __MLT_D3D_TEXTURE_H__

namespace mlt
{
	/**************************************************************************
	* 类声明
	**************************************************************************/

	class MD3DTexture : public ID3DTexture
	{
		friend class MD3DDevice;

	public:
		// 获取页面对象
		virtual LPDIRECT3DSURFACE9 GetSurface(UINT uLevel);
		// 获取描述信息
		virtual BOOL GetDesc(UINT uLevel, D3DSURFACE_DESC* pDesc);

	public:
		// 获取调色板
		virtual PPALETTEENTRY GetPalette() { return m_pPalette; }

		// 清空纹理页面数据
		virtual BOOL Clear(UINT uLevel, const RECT* pRect = NULL);

	public:
		// 加载页面数据
		//virtual BOOL LoadSurfaceFromFile(UINT uLevel, LPCTSTR pFile, //D3DXIMAGE_INFO* pImgInfo = NULL, 
		//						const RECT* pToRect = NULL, const RECT* pFromRect = NULL,
		//						D3DCOLOR dwColorKey = 0xFF000000, DWORD dwFilter = D3DX_DEFAULT);
		// 加载页面数据
		virtual BOOL LoadSurfaceFromMemory(UINT uLevel, LPCVOID pMemory, D3DFORMAT dwFormat, 
								const RECT* pRect, UINT uPitch, const PALETTEENTRY* pPalette = NULL, 
								const RECT* pStretchRect = NULL, D3DCOLOR dwColorKey = 0xFF000000, 
								DWORD dwFilter = D3DX_DEFAULT);
		// 加载页面数据
		virtual BOOL LoadSurfaceFromSurface(UINT uLevel, SP_ID3DTEXTURE spITex, 
								const RECT* pToRect = NULL, const RECT* pFromRect = NULL, 
								D3DCOLOR dwColorKey = 0xFF000000, DWORD dwFilter = D3DX_DEFAULT);
		
	protected:
		// 构造函数
		MD3DTexture(LPDIRECT3DTEXTURE9 pD3DTex9);

	private:
		// 校验矩形区域
		bool checkUpdateRect(UINT uLevel, long& X, long& Y, RECT& rcSrc);
		bool checkStretchRect(UINT uLevel, RECT& rcDest, RECT& rcSrc);

	private:
		// 调色板
		PPALETTEENTRY	m_pPalette;
	};
}


#endif // __MLT_D3D_TEXTURE_H__