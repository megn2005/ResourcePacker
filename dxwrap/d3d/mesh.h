/******************************************************************************
*	文件名	:	mesh.h
*	创建日期	:	7/2/2016
*	作者		:	Loser
*	描述		:	
******************************************************************************/

#ifndef __MLT_D3D_MESH_H__
#define __MLT_D3D_MESH_H__

namespace mlt
{
	/**************************************************************************
	* 类声明
	**************************************************************************/

	class MD3DMesh : public ID3DMesh
	{
		friend class MD3DDevice;

	public:
		// 释放对象
		virtual void Release();

		// 加载顶点、索引、属性缓存
		virtual BOOL LoadBuffer(const void* pVertexBuf, const void* pIndexBuf, const void* pAttriBuf);

		// 获取顶点缓存
		BOOL GetVertexBuffer(LPDIRECT3DVERTEXBUFFER9* ppVB);
		// 获取索引缓存
		BOOL GetIndexBuffer(LPDIRECT3DINDEXBUFFER9* ppIB);
		

		// 获取材质数量
		virtual DWORD GetMaterialNum() const { return m_dwMtrlNum; }
		// 获取材质
		virtual LPD3DXMATERIAL GetMaterial(DWORD dwId);

		// 绘制模型（子集）
		virtual BOOL DrawSubset(DWORD dwAttribId = 0);

	protected:
		// 构造函数
		MD3DMesh(LPD3DXMESH pD3DXMesh);

		MD3DMesh(LPD3DXMESH pD3DXMesh, 
			LPD3DXBUFFER pMaterials, DWORD dwNumMaterials, 
			LPD3DXBUFFER pAdjacency = NULL, LPD3DXBUFFER pEffectInsts = NULL);

	private:
		// 邻接信息
		LPD3DXBUFFER	m_pAdjacency;
		// 特殊效果
		LPD3DXBUFFER	m_pEffectInsts;

		// 材质数据
		LPD3DXBUFFER	m_pMaterials;
		// 材质数量
		DWORD			m_dwMtrlNum;

		
		
	};
}

#endif // __MLT_D3D_MESH_H__





