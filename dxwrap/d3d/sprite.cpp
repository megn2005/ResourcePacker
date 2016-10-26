/******************************************************************************
*	�ļ���	:	sprite.cpp
*	��������	:	7/2/2016
*	����		:	Loser
*	����		:	
******************************************************************************/

#include "../dxwrap.h"
#include "sprite.h"
#include "device.h"

namespace mlt
{
	//-------------------------------------------------------------------------
	// ����������
	//-------------------------------------------------------------------------
	MD3DSprite::~MD3DSprite()
	{
		MD3DDevice::s_Lost.disconnect(this);
		MD3DDevice::s_Reset.disconnect(this);
	}

	//-------------------------------------------------------------------------
	// ����任
	//-------------------------------------------------------------------------
	BOOL MD3DSprite::SetTransform(const D3DXMATRIX* pMatrix)
	{
		LPD3DXSPRITE pD3DXSprite = static_cast<LPD3DXSPRITE>(m_pIUnknown);
		if (!pD3DXSprite) return FALSE;

		//
		HRESULT hr = pD3DXSprite->SetTransform(pMatrix);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		return TRUE;
	}

	//-------------------------------------------------------------------------
	// ��ʼ����
	//-------------------------------------------------------------------------
	BOOL MD3DSprite::BeginDraw(DWORD dwFlags /*= 0*/)
	{
		LPD3DXSPRITE pD3DXSprite = static_cast<LPD3DXSPRITE>(m_pIUnknown);
		if (!pD3DXSprite) return FALSE;

		//
		HRESULT hr = pD3DXSprite->Begin(dwFlags);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		return TRUE;
	}

	//-------------------------------------------------------------------------
	// ��������
	//-------------------------------------------------------------------------
	BOOL MD3DSprite::EndDraw()
	{
		LPD3DXSPRITE pD3DXSprite = static_cast<LPD3DXSPRITE>(m_pIUnknown);
		if (!pD3DXSprite) return FALSE;
		
		//
		HRESULT hr = pD3DXSprite->Flush();
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		hr = pD3DXSprite->End();
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		return TRUE;
	}

	//-------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------
	BOOL MD3DSprite::Draw(SP_ID3DTEXTURE spITex, 
						const D3DXVECTOR3* pPosition /*= NULL*/,
						const RECT* pSrcRect /*= NULL*/, 
						const D3DXVECTOR3* pCenter /*= NULL*/,
						D3DCOLOR dwColor /*= 0xFFFFFFFF*/)
	{
		LPD3DXSPRITE pD3DXSprite = static_cast<LPD3DXSPRITE>(m_pIUnknown);
		if (!pD3DXSprite || !spITex) return FALSE;

		//
		HRESULT hr = pD3DXSprite->Draw(
						static_cast<LPDIRECT3DTEXTURE9>(spITex->GetInterface()), 
						pSrcRect, pCenter, pPosition, dwColor);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}
		
		return TRUE;
	}


	//-------------------------------------------------------------------------
	// �豸��ʧ�¼�
	//-------------------------------------------------------------------------
	void MD3DSprite::OnLostDevice()
	{
		LPD3DXSPRITE pD3DXSprite = static_cast<LPD3DXSPRITE>(m_pIUnknown);
		if (!pD3DXSprite) return ;

		//
		HRESULT hr = pD3DXSprite->OnLostDevice();
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return ;
		}
	}

	//-------------------------------------------------------------------------
	// �豸�����¼�
	//-------------------------------------------------------------------------
	void MD3DSprite::OnResetDevice()
	{
		LPD3DXSPRITE pD3DXSprite = static_cast<LPD3DXSPRITE>(m_pIUnknown);
		if (!pD3DXSprite) return ;

		//
		HRESULT hr = pD3DXSprite->OnResetDevice();
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return ;
		}
	}


	//-------------------------------------------------------------------------
	// ���캯��
	//-------------------------------------------------------------------------
	MD3DSprite::MD3DSprite(LPD3DXSPRITE pD3DXSprite)
		: ID3DSprite(pD3DXSprite)
	{
		MD3DDevice::s_Lost.connect(this, &MD3DSprite::OnLostDevice);
		MD3DDevice::s_Reset.connect(this, &MD3DSprite::OnResetDevice);
	}
	


	
}

