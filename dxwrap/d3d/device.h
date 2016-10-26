/******************************************************************************
*	文件名	:	device.h
*	创建日期	:	6/11/2016
*	作者		:	Loser
*	描述		:
******************************************************************************/

#ifndef	__MLT_D3D_DEVICE_H__
#define __MLT_D3D_DEVICE_H__

namespace mlt
{
	/**************************************************************************
	* 类声明
	**************************************************************************/

	class MD3DDevice : public ID3DDevice
	{
		friend class MDirect3D;

	public:
		// 创建空网格
		virtual SP_ID3DMESH CreateMeshFVF(DWORD dwNumFaces, DWORD dwNumVertices, DWORD dwFVF,
											DWORD dwOptions = D3DXMESH_MANAGED);
		// 从.X文件创建网格
		virtual SP_ID3DMESH CreateMeshFromX(LPCTSTR pPath);

		// 创建盒子（立方体）网格
		virtual SP_ID3DMESH CreateMeshBox(float fWidth, float fHeight, float fDepth);
		// 创建球面体网格
		virtual SP_ID3DMESH CreateMeshSphere(float fRadius, UINT uSlices, UINT uStacks);
		// 创建柱体网格
		virtual SP_ID3DMESH CreateMeshCylinder(float fRadius1, float fRadius2, 
											float fLength, UINT uSlices, UINT uStacks);
		// 创建茶壶网格
		virtual SP_ID3DMESH CreateMeshTeapot();
		// 创建多边形网格
		virtual SP_ID3DMESH CreateMeshPolygon(float fLength, UINT uSides);
		// 创建圆环体网格
		virtual SP_ID3DMESH CreateMeshTorus(float fInnerRadius, float fOuterRadius, 
											UINT uSides, UINT uRings); 


		// 创建纹理
		virtual SP_ID3DTEXTURE CreateTexture(UINT uWidth, UINT uHeight, 
											D3DFORMAT dwFormat = D3DFMT_A8R8G8B8, 
											D3DPOOL dwPool = D3DPOOL_MANAGED, 
											DWORD dwUsage = 0, UINT uLevels = 0);
		// 从文件创建纹理
		virtual SP_ID3DTEXTURE CreateTextureFromFile(LPCTSTR pPath);

		// 创建精灵
		virtual SP_ID3DSPRITE CreateSprite();

		// 创建字体
		virtual SP_ID3DFONT CreateFont(INT nHeight, UINT uWidth = 0, UINT uWeight = 0, 
									BOOL bItalic = FALSE, LPCTSTR pFaceName = TEXT("宋体"));

	public:
		// 矩阵（坐标系）变换
		virtual BOOL SetTransform(D3DTRANSFORMSTATETYPE enState, const D3DMATRIX* pMatrix);
		// 设置视口
		virtual BOOL SetViewport(DWORD dwX, DWORD dwY, DWORD dwWidth, DWORD dwHeight, 
									float fMinZ = 0.0f, float fMaxZ = 1.0f);

		// 设置材质
		virtual BOOL SetMaterial(D3DMATERIAL9* pD3DMtrl9);
		// 设置纹理
		virtual BOOL SetTexture(DWORD dwStage, SP_ID3DTEXTURE spITex);
		
		// 设置灯光
		virtual BOOL SetLight(DWORD dwIndex, const D3DLIGHT9* pLight);
		virtual BOOL LightEnable(DWORD dwLightIndex, BOOL bEnable = TRUE);
		
		// 设置渲染状态
		virtual BOOL SetRenderState(D3DRENDERSTATETYPE enState, DWORD dwValue);

		// 根据MDirect3D成员s_PP，重置设备
		virtual BOOL Reset();
		// 开始渲染
		virtual BOOL BeginRender(const D3DRECT* pRects = NULL, DWORD dwCount = 0, 
								DWORD dwFlags = D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
								D3DCOLOR dwColor = 0x00000000, float fZ = 1.0f, DWORD dwStencil = 0);
		// 结束渲染
		virtual BOOL EndRender();

	protected:
		// 构造函数
		MD3DDevice(LPDIRECT3DDEVICE9 pD3DDev9);

	public:
		// 设备丢失信号
		static signal0<>	s_Lost;
		// 设备重置信号
		static signal0<>	s_Reset;

	};



}




#endif	// __MLT_DX_DEVICE_H__






