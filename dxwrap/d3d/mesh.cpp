/******************************************************************************
*	�ļ���	:	mesh.cpp
*	��������	:	7/2/2016
*	����		:	Loser
*	����		:	
******************************************************************************/

#include "../dxwrap.h"
#include "mesh.h"

namespace mlt
{
	//-------------------------------------------------------------------------
	// �ͷŶ���
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
	// ��ȡ����
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
	// �����Ӽ�
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
	// ���캯��
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

