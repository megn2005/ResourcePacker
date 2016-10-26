/******************************************************************************
*	�ļ���	:	texture.cpp
*	��������	:	7/10/2016
*	����		:	Loser
*	����		:	
******************************************************************************/

#include "../dxwrap.h"
#include "texture.h"

namespace mlt
{
	//-------------------------------------------------------------------------
	// ��ȡҳ�����
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
			// ʧ�ܴ���
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
		}

		return pD3DSurface9;
	}

	//-------------------------------------------------------------------------
	// ��ȡ������Ϣ
	//-------------------------------------------------------------------------
	BOOL MD3DTexture::GetDesc(UINT uLevel, D3DSURFACE_DESC* pDesc)
	{
		LPDIRECT3DTEXTURE9 pD3DTex9 = static_cast<LPDIRECT3DTEXTURE9>(m_pIUnknown);
		if (!pD3DTex9 || !pDesc) return FALSE;

		// 
		HRESULT hr = pD3DTex9->GetLevelDesc(uLevel, pDesc);
		if (FAILED(hr))
		{
			// ʧ�ܴ���
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		return TRUE;
	}


	//-------------------------------------------------------------------------
	// �������ҳ������
	//-------------------------------------------------------------------------
	BOOL MD3DTexture::Clear(UINT uLevel, const RECT* pRect /*= NULL*/)
	{
		LPDIRECT3DTEXTURE9 pD3DTex9 = static_cast<LPDIRECT3DTEXTURE9>(m_pIUnknown);
		if (!pD3DTex9) return FALSE;

		// �����������
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

		// ��ҳ��
		D3DLOCKED_RECT stLockRect = {0};
		
		HRESULT hr = pD3DTex9->LockRect(uLevel, &stLockRect, pRect, D3DLOCK_NOSYSLOCK);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		// ���ҳ��
		ZeroMemory(stLockRect.pBits, (rcClear.bottom - rcClear.top) * stLockRect.Pitch);

		// ����ҳ��
		hr = pD3DTex9->UnlockRect(uLevel);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		return TRUE;
	}


	//-------------------------------------------------------------------------
	// ����ҳ������
	//-------------------------------------------------------------------------
	BOOL MD3DTexture::LoadSurfaceFromMemory(UINT uLevel, LPCVOID pMemory, D3DFORMAT dwFormat, 
								const RECT* pRect, UINT uPitch, const PALETTEENTRY* pPalette /*= NULL*/, 
								const RECT* pStretchRect /*= NULL*/, D3DCOLOR dwColorKey /*= 0xFF000000*/, 
								DWORD dwFilter /*= D3DX_DEFAULT*/)
	{
		// ��ȡҳ�����
		LPDIRECT3DSURFACE9 pD3DSurface9 = GetSurface(uLevel);
		if (!pD3DSurface9) return FALSE;
		
		// У���������
		RECT rcSrc = *pRect;
		RECT rcDest = *pStretchRect;

		if (!checkStretchRect(nLevel, rcDest, rcSrc))
			return FALSE;

		// ����ҳ������
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
	// ����ҳ������
	//-------------------------------------------------------------------------
	BOOL MD3DTexture::LoadSurfaceFromSurface(UINT uLevel, SP_ID3DTEXTURE spITex, 
								const RECT* pToRect /*= NULL*/, const RECT* pFromRect /*= NULL*/, 
								D3DCOLOR dwColorKey /*= 0xFF000000*/, DWORD dwFilter /*= D3DX_DEFAULT*/)
	{
		// ��ȡҳ�����
		LPDIRECT3DSURFACE9 pD3DSurface9 = GetSurface(uLevel);
		if (!pD3DSurface9) return FALSE;
		
		// ����ҳ��
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
	// ���캯��
	//-------------------------------------------------------------------------
	MD3DTexture::MD3DTexture(LPDIRECT3DTEXTURE9 pD3DTex9)
		: ID3DTexture(pD3DTex9), m_pPalette(NULL)
	{
	}


	//-------------------------------------------------------------------------
	// ����: ��Դͼ�����rcSrc���Ƶ�Ŀ��ͼ�����rcDest��(X,Y)��,�˺���
	//		���޸�Դͼ����κ�Ŀ��ͼ����ε���Ч����.
	// Ŀ��: ����UpdateSurface����,У��Դͼ��rcSrc����,�õ�Ҫ���µ�Ŀ��
	//		rcDest����.
	//-------------------------------------------------------------------------
	bool MD3DTexture::checkUpdateRect(UINT uLevel, long& X, long& Y, RECT& rcSrc)
	{
		// ��ȡҳ������
		D3DSURFACE_DESC desc;
		
		if (!GetDesc(uLevel, &desc))
			return false;

		// Ŀ�ľ���
		RECT rcDest = {0, 0, desc.Width, desc.Height};

		// У��Դ����

		// X < 0 or Y < 0
		if ( X < 0 ) { rcSrc.left -= X; X = 0;	}
		if ( Y < 0 ) { rcSrc.top -= Y; Y = 0;	}

		// right - left > width or bottom - top > height
		rcSrc.right	= min( rcDest.right - rcDest.left - X + rcSrc.left, rcSrc.right );
		rcSrc.bottom = min( rcDest.bottom - rcDest.top - Y + rcSrc.top, rcSrc.bottom );

		return true;
	}

	//-------------------------------------------------------------------------
	// ��Ҫ���ں���LoadSurfaceFromSurface(...)�ľ��β���У��; ����width��
	// height����������,Դ���δ�С�������������(0,0,width,height)������.
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

