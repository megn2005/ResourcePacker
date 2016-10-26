/******************************************************************************
*	�ļ���	:	mesh.h
*	��������	:	7/2/2016
*	����		:	Loser
*	����		:	
******************************************************************************/

#ifndef __MLT_D3D_MESH_H__
#define __MLT_D3D_MESH_H__

namespace mlt
{
	/**************************************************************************
	* ������
	**************************************************************************/

	class MD3DMesh : public ID3DMesh
	{
		friend class MD3DDevice;

	public:
		// �ͷŶ���
		virtual void Release();

		// ���ض��㡢���������Ի���
		virtual BOOL LoadBuffer(const void* pVertexBuf, const void* pIndexBuf, const void* pAttriBuf);

		// ��ȡ���㻺��
		BOOL GetVertexBuffer(LPDIRECT3DVERTEXBUFFER9* ppVB);
		// ��ȡ��������
		BOOL GetIndexBuffer(LPDIRECT3DINDEXBUFFER9* ppIB);
		

		// ��ȡ��������
		virtual DWORD GetMaterialNum() const { return m_dwMtrlNum; }
		// ��ȡ����
		virtual LPD3DXMATERIAL GetMaterial(DWORD dwId);

		// ����ģ�ͣ��Ӽ���
		virtual BOOL DrawSubset(DWORD dwAttribId = 0);

	protected:
		// ���캯��
		MD3DMesh(LPD3DXMESH pD3DXMesh);

		MD3DMesh(LPD3DXMESH pD3DXMesh, 
			LPD3DXBUFFER pMaterials, DWORD dwNumMaterials, 
			LPD3DXBUFFER pAdjacency = NULL, LPD3DXBUFFER pEffectInsts = NULL);

	private:
		// �ڽ���Ϣ
		LPD3DXBUFFER	m_pAdjacency;
		// ����Ч��
		LPD3DXBUFFER	m_pEffectInsts;

		// ��������
		LPD3DXBUFFER	m_pMaterials;
		// ��������
		DWORD			m_dwMtrlNum;

		
		
	};
}

#endif // __MLT_D3D_MESH_H__





