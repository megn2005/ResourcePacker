/******************************************************************************
*	文件名	:	mesh.cpp
*	创建日期	:	7/2/2016
*	作者		:	Loser
*	描述		:	
******************************************************************************/

#include "../dxwrap.h"
#include "mesh.h"

namespace mlt
{
	//-------------------------------------------------------------------------
	// 释放对象
	//-------------------------------------------------------------------------
	void MD3DMesh::Release()
	{
		SAFE_RELEASE(m_pAdjacency);
		SAFE_RELEASE(m_pEffectInsts);
		SAFE_RELEASE(m_pMaterials);
		m_dwMtrlNum = 0;

		ID3DMesh::Release();
	}


	//-------------------------------------------------------------------------
	// 获取材质
	//-------------------------------------------------------------------------
	LPD3DXMATERIAL MD3DMesh::GetMaterial(DWORD dwId)
	{
		//ASSERT(NULL != m_pMaterials && id < m_dwMtrlNum);
		if (m_pMaterials && dwId < m_dwMtrlNum)
		{
			return ((LPD3DXMATERIAL)m_pMaterials->GetBufferPointer()) + dwId;
		}

		return NULL;
	}


	//-------------------------------------------------------------------------
	// 绘制子集
	//-------------------------------------------------------------------------
	BOOL MD3DMesh::DrawSubset(DWORD dwAttribId /*= 0*/)
	{
		LPD3DXMESH pD3DXMesh = static_cast<LPD3DXMESH>(m_pIUnknown);
		if (!pD3DXMesh) return FALSE;
		
		//
		HRESULT hr = pD3DXMesh->DrawSubset(dwAttribId);
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

	MD3DMesh::MD3DMesh(LPD3DXMESH pD3DXMesh) 
		: ID3DMesh(pD3DXMesh), m_pAdjacency(NULL), m_pEffectInsts(NULL), 
		m_pMaterials(NULL), m_dwMtrlNum(0)
	{
	}

	MD3DMesh::MD3DMesh(LPD3DXMESH pD3DXMesh, 
		LPD3DXBUFFER pMaterials, DWORD dwNumMaterials, 
		LPD3DXBUFFER pAdjacency /*= NULL*/, LPD3DXBUFFER pEffectInsts /*= NULL*/)
		: ID3DMesh(pD3DXMesh), m_pAdjacency(pAdjacency), m_pEffectInsts(pEffectInsts), 
		m_pMaterials(pMaterials), m_dwMtrlNum(dwNumMaterials)
	{
	}

	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
}

