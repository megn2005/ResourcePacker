/******************************************************************************
*	文件名	:	device.cpp
*	创建日期	:	6/12/2016
*	作者		:	Loser
*	描述		:
******************************************************************************/

#include "../dxwrap.h"
#include "device.h"
#include "direct3d.h"
#include "mesh.h"
#include "texture.h"
#include "sprite.h"
#include "font.h"

namespace mlt
{
	// 设备丢失信号
	signal0<> MD3DDevice::s_Lost;
	// 设备重置信号
	signal0<> MD3DDevice::s_Reset;


	//-------------------------------------------------------------------------
	// 创建空网格
	//-------------------------------------------------------------------------
	SP_ID3DMESH MD3DDevice::CreateMeshFVF(DWORD dwNumFaces, DWORD dwNumVertices, DWORD dwFVF,
											DWORD dwOptions /*= D3DXMESH_MANAGED*/)
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = static_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return NULL;

		//
		LPD3DXMESH pD3DXMesh = NULL;

		HRESULT hr = D3DXCreateMeshFVF(dwNumFaces, dwNumVertices, dwOptions, dwFVF, 
												pD3DDev9, &pD3DXMesh);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return NULL;
		}

		return SP_ID3DMESH(new MD3DMesh(pD3DXMesh));
	}

	//-------------------------------------------------------------------------
	// 从.X文件创建网格
	//-------------------------------------------------------------------------
	SP_ID3DMESH MD3DDevice::CreateMeshFromX(LPCTSTR pPath)
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = static_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return NULL;

		//
		LPD3DXBUFFER pAdjacency = NULL;
		LPD3DXBUFFER pMaterials = NULL;
		LPD3DXBUFFER pEffectInsts = NULL;
		DWORD dwNumMaterials = 0;
		LPD3DXMESH pD3DXMesh = NULL;

		HRESULT hr = D3DXLoadMeshFromX(pPath, D3DXMESH_MANAGED, pD3DDev9, 
						&pAdjacency, &pMaterials, &pEffectInsts, &dwNumMaterials, 
						&pD3DXMesh);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return NULL;
		}	

		return SP_ID3DMESH(new MD3DMesh(pD3DXMesh, 
						pMaterials, dwNumMaterials, pAdjacency, pEffectInsts));
	}


	//-------------------------------------------------------------------------
	// 创建盒子（立方体）网格
	//-------------------------------------------------------------------------
	SP_ID3DMESH MD3DDevice::CreateMeshBox(float fWidth, float fHeight, float fDepth)
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = static_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return NULL;
		
		//
		LPD3DXMESH pD3DXMesh = NULL;

		HRESULT hr = D3DXCreateBox(pD3DDev9, fWidth, fHeight, fDepth, &pD3DXMesh, NULL);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return NULL;
		}

		return SP_ID3DMESH(new MD3DMesh(pD3DXMesh));
	}

	//-------------------------------------------------------------------------
	// 创建球面体网格
	//-------------------------------------------------------------------------
	SP_ID3DMESH MD3DDevice::CreateMeshSphere(float fRadius, UINT uSlices, UINT uStacks)
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = static_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return NULL;

		// 
		LPD3DXMESH pD3DXMesh = NULL;

		HRESULT hr = D3DXCreateSphere(pD3DDev9, fRadius, uSlices, uStacks, &pD3DXMesh, NULL);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return NULL;
		}

		return SP_ID3DMESH(new MD3DMesh(pD3DXMesh));
	}

	//-------------------------------------------------------------------------
	// 创建柱体网格
	//-------------------------------------------------------------------------
	SP_ID3DMESH MD3DDevice::CreateMeshCylinder(float fRadius1, float fRadius2, 
											float fLength, UINT uSlices, UINT uStacks)
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = static_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return NULL;

		//
		LPD3DXMESH pD3DXMesh = NULL;

		HRESULT hr = D3DXCreateCylinder(pD3DDev9, fRadius1, fRadius2, 
									fLength, uSlices, uStacks, &pD3DXMesh, NULL);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return NULL;
		}
		
		return SP_ID3DMESH(new MD3DMesh(pD3DXMesh));
	}

	//-------------------------------------------------------------------------
	// 创建茶壶网格
	//-------------------------------------------------------------------------
	SP_ID3DMESH MD3DDevice::CreateMeshTeapot()
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = static_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return NULL;

		// 
		LPD3DXMESH pD3DXMesh = NULL;

		HRESULT hr = D3DXCreateTeapot(pD3DDev9, &pD3DXMesh, NULL);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return NULL;
		}

		return SP_ID3DMESH(new MD3DMesh(pD3DXMesh));
	}

	//-------------------------------------------------------------------------
	// 创建多边形网格
	//-------------------------------------------------------------------------
	SP_ID3DMESH MD3DDevice::CreateMeshPolygon(float fLength, UINT uSides)
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = static_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return NULL;

		// 
		LPD3DXMESH pD3DXMesh = NULL;

		HRESULT hr = D3DXCreatePolygon(pD3DDev9, fLength, uSides, &pD3DXMesh, NULL);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return NULL;
		}

		return SP_ID3DMESH(new MD3DMesh(pD3DXMesh));
	}

	//-------------------------------------------------------------------------
	// 创建圆环体网格
	//-------------------------------------------------------------------------
	SP_ID3DMESH MD3DDevice::CreateMeshTorus(float fInnerRadius, float fOuterRadius, 
											UINT uSides, UINT uRings)
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = static_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return NULL;

		//
		LPD3DXMESH pD3DXMesh = NULL;

		HRESULT hr = D3DXCreateTorus(pD3DDev9, fInnerRadius, fOuterRadius, uSides, uRings, 
									&pD3DXMesh, NULL);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return NULL;
		}

		return SP_ID3DMESH(new MD3DMesh(pD3DXMesh));
	}


	//-------------------------------------------------------------------------
	// 创建纹理
	//-------------------------------------------------------------------------
	SP_ID3DTEXTURE MD3DDevice::CreateTexture(UINT uWidth, UINT uHeight, 
											D3DFORMAT dwFormat /*= D3DFMT_A8R8G8B8*/, 
											D3DPOOL dwPool /*= D3DPOOL_MANAGED*/, 
											DWORD dwUsage /*= 0*/, UINT uLevels /*= 0*/)
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = static_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return NULL;

		//
		LPDIRECT3DTEXTURE9 pD3DTex9 = NULL;

		HRESULT hr = pD3DDev9->CreateTexture(uWidth, uHeight, uLevels, 
										dwUsage, dwFormat, dwPool, &pD3DTex9, NULL);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return NULL;
		}

		return SP_ID3DTEXTURE(new MD3DTexture(pD3DTex9));
	}

	//-------------------------------------------------------------------------
	// 从文件创建纹理
	//-------------------------------------------------------------------------
	SP_ID3DTEXTURE MD3DDevice::CreateTextureFromFile(LPCTSTR pPath)
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = static_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return NULL;

		// 
		LPDIRECT3DTEXTURE9 pD3DTex9 = NULL;

		HRESULT hr = D3DXCreateTextureFromFile(pD3DDev9, pPath, &pD3DTex9);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return NULL;
		}

		return SP_ID3DTEXTURE(new MD3DTexture(pD3DTex9));
	}


	//-------------------------------------------------------------------------
	// 创建精灵
	//-------------------------------------------------------------------------
	SP_ID3DSPRITE MD3DDevice::CreateSprite()
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = static_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return NULL;

		//
		LPD3DXSPRITE pD3DXSprite = NULL;

		HRESULT hr = D3DXCreateSprite(pD3DDev9, &pD3DXSprite);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return NULL;
		}

		return SP_ID3DSPRITE(new MD3DSprite(pD3DXSprite));
	}


	//-------------------------------------------------------------------------
	// 创建字体
	//-------------------------------------------------------------------------
	SP_ID3DFONT MD3DDevice::CreateFont(INT nHeight, UINT uWidth /*= 0*/, 
									UINT uWeight /*= 0*/, BOOL bItalic /*= FALSE*/, 
									LPCTSTR pFaceName /*= TEXT("宋体")*/)
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = dynamic_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return NULL;

		// 
		LPD3DXFONT pD3DXFont = NULL;

		HRESULT hr = D3DXCreateFont(pD3DDev9, nHeight, uWidth, uWeight, 1, 
									bItalic, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
									DEFAULT_QUALITY, 0, pFaceName, &pD3DXFont);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return NULL;
		}

		return SP_ID3DFONT(new MD3DFont(pD3DXFont));
	}


	//-------------------------------------------------------------------------
	// 矩阵（坐标系）变换
	//-------------------------------------------------------------------------
	BOOL MD3DDevice::SetTransform(D3DTRANSFORMSTATETYPE enState, const D3DMATRIX* pMatrix)
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = dynamic_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return FALSE;
		
		HRESULT hr = pD3DDev9->SetTransform(enState, pMatrix);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		return TRUE;
	}

	//-------------------------------------------------------------------------
	// 设置视口
	//-------------------------------------------------------------------------
	BOOL MD3DDevice::SetViewport(DWORD dwX, DWORD dwY, DWORD dwWidth, DWORD dwHeight, 
									float fMinZ /*= 0.0f*/, float fMaxZ /*= 1.0f*/)
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = dynamic_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return FALSE;

		D3DVIEWPORT9 vp = { dwX, dwY, dwWidth, dwHeight, fMinZ, fMaxZ };
		HRESULT hr = pD3DDev9->SetViewport(&vp);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		return TRUE;
	}


	//-------------------------------------------------------------------------
	// 设置材质
	//-------------------------------------------------------------------------
	BOOL MD3DDevice::SetMaterial(D3DMATERIAL9* pD3DMtrl9)
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = dynamic_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return FALSE;
		
		HRESULT hr = pD3DDev9->SetMaterial(pD3DMtrl9);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		return TRUE;
	}

	//-------------------------------------------------------------------------
	// 设置纹理
	//-------------------------------------------------------------------------
	BOOL MD3DDevice::SetTexture(DWORD dwStage, SP_ID3DTEXTURE spITex)
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = dynamic_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return FALSE;

		//
		HRESULT hr = pD3DDev9->SetTexture(dwStage, spITex ? 
				dynamic_cast<LPDIRECT3DTEXTURE9>(spITex->GetInterface()) : NULL);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		return TRUE;
	}


	//----------------------------------------------------------------------------
	// 设置灯光
	//----------------------------------------------------------------------------
	BOOL MD3DDevice::SetLight(DWORD dwIndex, const D3DLIGHT9* pLight)
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = dynamic_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return FALSE;

		//
		HRESULT hr = pD3DDev9->SetLight(dwIndex, pLight);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		return TRUE;
	}

	BOOL MD3DDevice::LightEnable(DWORD dwLightIndex, BOOL bEnable /*= TRUE*/)
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = dynamic_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return FALSE;

		//
		HRESULT hr = pD3DDev9->LightEnable(dwLightIndex, bEnable);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		return TRUE;
	}


	//-------------------------------------------------------------------------
	// 设置渲染状态
	//-------------------------------------------------------------------------
	BOOL MD3DDevice::SetRenderState(D3DRENDERSTATETYPE enState, DWORD dwValue)
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = dynamic_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return FALSE;

		HRESULT hr = pD3DDev9->SetRenderState(enState, dwValue);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		return TRUE;
	}


	//-------------------------------------------------------------------------
	// 根据MDirect3D成员s_PP，重置设备
	//-------------------------------------------------------------------------
	BOOL MD3DDevice::Reset()
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = static_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return FALSE;
		
		//
		s_Lost();

		//
		HRESULT hr = pD3DDev9->Reset(&MDirect3D::s_PP);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		//
		s_Reset();
		
		return TRUE;
	}

	//-------------------------------------------------------------------------
	// 开始渲染
	//-------------------------------------------------------------------------
	BOOL MD3DDevice::BeginRender(const D3DRECT* pRects /*= NULL*/, DWORD dwCount /*= 0*/, 
								DWORD dwFlags /*= D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER*/, 
								D3DCOLOR dwColor /*= 0x00000000*/, float fZ /*= 1.0f*/, DWORD dwStencil /*= 0*/)
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = static_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return FALSE;
		
		// 
		HRESULT hr = pD3DDev9->Clear(dwCount, pRects, dwFlags, dwColor, fZ, dwStencil);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		// 
		hr = pD3DDev9->BeginScene();
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		return TRUE;
	}

	//-------------------------------------------------------------------------
	// 结束渲染
	//-------------------------------------------------------------------------
	BOOL MD3DDevice::EndRender()
	{
		LPDIRECT3DDEVICE9 pD3DDev9 = static_cast<LPDIRECT3DDEVICE9>(m_pIUnknown);
		if (!pD3DDev9) return FALSE;
		
		//
		HRESULT hr = pD3DDev9->EndScene();
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		//
		hr = pD3DDev9->Present(0, 0, MDirect3D::s_PP.hDeviceWindow, 0);
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
	MD3DDevice::MD3DDevice(LPDIRECT3DDEVICE9 pD3DDev9) 
		: ID3DDevice(pD3DDev9)
	{
	}


	

}
