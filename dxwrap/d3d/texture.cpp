/******************************************************************************
*	文件名	:	texture.cpp
*	创建日期	:	7/10/2016
*	作者		:	Loser
*	描述		:	
******************************************************************************/

#include "../dxwrap.h"
#include "texture.h"

namespace mlt
{
	//-------------------------------------------------------------------------
	// 获取页面对象
	//-------------------------------------------------------------------------
	LPDIRECT3DSURFACE9 MD3DTexture::GetSurface(UINT uLevel)
	{
		//LPDIRECT3DTEXTURE9 pD3DTex9 = dynamic_cast<LPDIRECT3DTEXTURE9>(m_pIUnknown);
		LPDIRECT3DTEXTURE9 pD3DTex9 = static_cast<LPDIRECT3DTEXTURE9>(m_pIUnknown);
		if (!pD3DTex9) return NULL;

		//
		LPDIRECT3DSURFACE9 pD3DSurface9 = NULL;

		HRESULT hr = pD3DTex9->GetSurfaceLevel(uLevel, &pD3DSurface9);
		if (FAILED(hr))
		{
			// 失败处理
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
		}

		return pD3DSurface9;
	}

	//-------------------------------------------------------------------------
	// 获取描述信息
	//-------------------------------------------------------------------------
	BOOL MD3DTexture::GetDesc(UINT uLevel, D3DSURFACE_DESC* pDesc)
	{
		LPDIRECT3DTEXTURE9 pD3DTex9 = static_cast<LPDIRECT3DTEXTURE9>(m_pIUnknown);
		if (!pD3DTex9 || !pDesc) return FALSE;

		// 
		HRESULT hr = pD3DTex9->GetLevelDesc(uLevel, pDesc);
		if (FAILED(hr))
		{
			// 失败处理
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		return TRUE;
	}


	//-------------------------------------------------------------------------
	// 清空纹理页面数据
	//-------------------------------------------------------------------------
	BOOL MD3DTexture::Clear(UINT uLevel, const RECT* pRect /*= NULL*/)
	{
		LPDIRECT3DTEXTURE9 pD3DTex9 = static_cast<LPDIRECT3DTEXTURE9>(m_pIUnknown);
		if (!pD3DTex9) return FALSE;

		// 计算清理矩形
		RECT rcClear = {0};

		if (!pRect)
		{
			D3DSURFACE_DESC desc;
			if (GetDesc(uLevel, &desc))
			{
				rcClear.right = desc.Width;
				rcClear.bottom = desc.Height;
			}
			else
				return FALSE;
		}
		else
		{
			rcClear = *pRect;
			if (!checkUpdateRect(uLevel, rcClear.left, rcClear.top, rcClear))
				return FALSE;
		}

		// 锁页面
		D3DLOCKED_RECT stLockRect = {0};
		
		HRESULT hr = pD3DTex9->LockRect(uLevel, &stLockRect, pRect, D3DLOCK_NOSYSLOCK);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		// 清空页面
		ZeroMemory(stLockRect.pBits, (rcClear.bottom - rcClear.top) * stLockRect.Pitch);

		// 解锁页面
		hr = pD3DTex9->UnlockRect(uLevel);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		return TRUE;
	}


	//-------------------------------------------------------------------------
	// 加载页面数据
	//-------------------------------------------------------------------------
	BOOL MD3DTexture::LoadSurfaceFromMemory(UINT uLevel, LPCVOID pMemory, D3DFORMAT dwFormat, 
								const RECT* pRect, UINT uPitch, const PALETTEENTRY* pPalette /*= NULL*/, 
								const RECT* pStretchRect /*= NULL*/, D3DCOLOR dwColorKey /*= 0xFF000000*/, 
								DWORD dwFilter /*= D3DX_DEFAULT*/)
	{
		// 获取页面对象
		LPDIRECT3DSURFACE9 pD3DSurface9 = GetSurface(uLevel);
		if (!pD3DSurface9) return FALSE;
		
		// 校验矩形区域
		RECT rcSrc = *pRect;
		RECT rcDest = *pStretchRect;

		if (!checkStretchRect(nLevel, rcDest, rcSrc))
			return FALSE;

		// 加载页面数据
		HRESULT hr = D3DXLoadSurfaceFromMemory(pD3DSurface9, m_pPalette, &rcDest, 
											pMemory, dwFormat, uPitch, pPalette, &rcSrc, 
											dwFilter, dwColorKey);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		return TRUE;
	}

	//-------------------------------------------------------------------------
	// 加载页面数据
	//-------------------------------------------------------------------------
	BOOL MD3DTexture::LoadSurfaceFromSurface(UINT uLevel, SP_ID3DTEXTURE spITex, 
								const RECT* pToRect /*= NULL*/, const RECT* pFromRect /*= NULL*/, 
								D3DCOLOR dwColorKey /*= 0xFF000000*/, DWORD dwFilter /*= D3DX_DEFAULT*/)
	{
		// 获取页面对象
		LPDIRECT3DSURFACE9 pD3DSurface9 = GetSurface(uLevel);
		if (!pD3DSurface9) return FALSE;
		
		// 拷贝页面
		HRESULT hr = D3DXLoadSurfaceFromSurface(pD3DSurface9, m_pPalette, pToRect, 
										spITex->GetSurface(uLevel), spITex->GetPalette(), pFromRect, 
										dwFilter, dwColorKey);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}
		
		return TRUE;
	}


	//-------------------------------------------------------------------------
	// 构造函数
	//-------------------------------------------------------------------------
	MD3DTexture::MD3DTexture(LPDIRECT3DTEXTURE9 pD3DTex9)
		: ID3DTexture(pD3DTex9), m_pPalette(NULL)
	{
	}


	//-------------------------------------------------------------------------
	// 作用: 将源图像矩形rcSrc绘制到目的图像矩形rcDest的(X,Y)点,此函数
	//		会修改源图像矩形和目的图像矩形的有效区域.
	// 目的: 由于UpdateSurface限制,校验源图像rcSrc矩形,得到要更新的目标
	//		rcDest矩形.
	//-------------------------------------------------------------------------
	bool MD3DTexture::checkUpdateRect(UINT uLevel, long& X, long& Y, RECT& rcSrc)
	{
		// 获取页面描述
		D3DSURFACE_DESC desc;
		
		if (!GetDesc(uLevel, &desc))
			return false;

		// 目的矩形
		RECT rcDest = {0, 0, desc.Width, desc.Height};

		// 校验源矩形

		// X < 0 or Y < 0
		if ( X < 0 ) { rcSrc.left -= X; X = 0;	}
		if ( Y < 0 ) { rcSrc.top -= Y; Y = 0;	}

		// right - left > width or bottom - top > height
		rcSrc.right	= min( rcDest.right - rcDest.left - X + rcSrc.left, rcSrc.right );
		rcSrc.bottom = min( rcDest.bottom - rcDest.top - Y + rcSrc.top, rcSrc.bottom );

		return true;
	}

	//-------------------------------------------------------------------------
	// 主要用于函数LoadSurfaceFromSurface(...)的矩形参数校验; 由于width和
	// height参数的限制,源矩形大小不能拉伸出矩形(0,0,width,height)的外面.
	//-------------------------------------------------------------------------
	bool MD3DTexture::checkStretchRect(UINT uLevel, RECT& rcDest, RECT& rcSrc)
	{
		RECT rcOldRect	= rcDest;

		float fScaleW	= (float)( rcSrc.right - rcSrc.left ) / (float)( rcDest.right - rcDest.left );
		float fScaleH	= (float)( rcSrc.bottom - rcSrc.top ) / (float)( rcDest.bottom - rcDest.top );

		if (!checkUpdateRect(uLevel, rcDest.left, rcDest.top, rcDest))
			return false;

		rcSrc.left		= (long)(( rcDest.left - rcOldRect.left ) * fScaleW );
		rcSrc.right		= (long)(( rcDest.right - rcOldRect.left ) * fScaleW );
		rcSrc.top		= (long)(( rcDest.top - rcOldRect.top ) * fScaleH );
		rcSrc.bottom	= (long)(( rcDest.bottom - rcOldRect.top ) * fScaleH );

		return true;
	}
	
}

