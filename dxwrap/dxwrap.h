/******************************************************************************
*	文件名:		dxwrap.h
*	创建日期:		6/17/2016
*	作者:		Loser
*	描述:		
******************************************************************************/

#ifndef	__MLT_DX_WRAP_H__
#define __MLT_DX_WRAP_H__

#include <memory>
using namespace std::tr1;

#ifdef _DEBUG
	#define D3D_DEBUG_INFO
#endif

#include <d3d9.h>
#include <d3dx9.h>

#ifdef DXWRAP_EXPORTS
	#include <dxerr.h>
	
	#pragma comment(lib, "d3d9.lib")
	#pragma comment(lib, "d3dx9.lib")
	#pragma comment(lib, "dxerr.lib")
#endif

// 信号槽
#include "sigslot.h"
using namespace sigslot;

namespace mlt
{
	/**************************************************************************
	* 
	**************************************************************************/

	// 导入导出
	#ifdef	DXWRAP_EXPORTS
		#define	DLL_CLASS	class __declspec(dllexport)
		#define	DLL_FUNC	__declspec(dllexport)
	#else
		#define	DLL_CLASS	class __declspec(dllimport)
		#define	DLL_FUNC	__declspec(dllimport)
	#endif

	// 安全释放
	#define SAFE_RELEASE(ptr)	if (ptr) { ptr->Release(); ptr = NULL; }


	/*****************************************************************************
	*
	*****************************************************************************/

	// 接口声明
	class ID3DTexture;
	class ID3DDevice;
	class ID3DMesh;
	class ID3DTexture;
	class ID3DSprite;
	class ID3DFont;

	// 共享指针类型
	typedef shared_ptr<ID3DDevice>		SP_ID3DDEVICE;
	typedef shared_ptr<ID3DMesh>		SP_ID3DMESH;
	typedef shared_ptr<ID3DTexture>		SP_ID3DTEXTURE;
	typedef shared_ptr<ID3DSprite>		SP_ID3DSPRITE;
	typedef shared_ptr<ID3DFont>		SP_ID3DFONT;
	
	
	/**************************************************************************
	* 接口基类
	**************************************************************************/

	class ID3DBase
	{
	public:
		// 构造函数
		ID3DBase(LPUNKNOWN pIUnknown) 
			: m_pIUnknown(pIUnknown)
		{
		}

		// 虚析构函数
		virtual ~ID3DBase() 
		{ 
			Release(); 
		}

		// 释放对象
		virtual void Release() 
		{ 
			SAFE_RELEASE(m_pIUnknown); 
		}

		// 获取接口指针
		virtual LPUNKNOWN GetInterface() 
		{ 
			return m_pIUnknown; 
		} 

	protected:
		// 接口指针
		LPUNKNOWN	m_pIUnknown;
	};


	/**************************************************************************
	* 接口声明
	**************************************************************************/

	// 设备接口
	class ID3DDevice : public ID3DBase
	{
	public:
		// 创建网格
		virtual SP_ID3DMESH CreateMesh() = 0;
		// 从.X文件创建网格
		virtual SP_ID3DMESH CreateMeshFromX(LPCTSTR pPath) = 0;

		// 创建盒子（立方体）网格
		virtual SP_ID3DMESH CreateMeshBox(FLOAT fWidth, FLOAT fHeight, FLOAT fDepth) = 0;
		// 创建球面体网格
		virtual SP_ID3DMESH CreateMeshSphere(FLOAT fRadius, UINT uSlices, UINT uStacks) = 0;
		// 创建柱体网格
		virtual SP_ID3DMESH CreateMeshCylinder(FLOAT fRadius1, FLOAT fRadius2, 
											FLOAT fLength, UINT uSlices, UINT uStacks) = 0;
		// 创建茶壶网格
		virtual SP_ID3DMESH CreateMeshTeapot() = 0;
		// 创建多边形网格
		virtual SP_ID3DMESH CreateMeshPolygon(FLOAT fLength, UINT uSides) = 0;
		// 创建圆环体网格
		virtual SP_ID3DMESH CreateMeshTorus(FLOAT fInnerRadius, FLOAT fOuterRadius, 
											UINT uSides, UINT uRings) = 0; 

		// 创建纹理
		virtual SP_ID3DTEXTURE CreateTexture(UINT uWidth, UINT uHeight, 
											D3DFORMAT dwFormat = D3DFMT_A8R8G8B8, 
											D3DPOOL dwPool = D3DPOOL_MANAGED, 
											DWORD dwUsage = 0, UINT uLevels = 0) = 0;
		// 从文件创建纹理
		virtual SP_ID3DTEXTURE CreateTextureFromFile(LPCTSTR pPath) = 0;

		// 创建精灵
		virtual SP_ID3DSPRITE CreateSprite() = 0;

		// 创建字体
		virtual SP_ID3DFONT CreateFont(INT nHeight, UINT uWidth = 0, UINT uWeight = 0, 
									BOOL bItalic = FALSE, LPCTSTR pFaceName = TEXT("宋体")) = 0;

	public:
		// 矩阵（坐标系）变换
		virtual BOOL SetTransform(D3DTRANSFORMSTATETYPE enState, const D3DMATRIX* pMatrix) = 0;
		// 设置视口
		virtual BOOL SetViewport(DWORD dwX, DWORD dwY, DWORD dwWidth, DWORD dwHeight, 
									float fMinZ = 0.0f, float fMaxZ = 1.0f) = 0;

		// 设置材质
		virtual BOOL SetMaterial(D3DMATERIAL9* pD3DMtrl9) = 0;
		// 设置纹理
		virtual BOOL SetTexture(DWORD dwStage, SP_ID3DTEXTURE spITex) = 0;

		// 设置灯光
		virtual BOOL SetLight(DWORD dwIndex, const D3DLIGHT9* pLight) = 0;
		virtual BOOL LightEnable(DWORD dwLightIndex, BOOL bEnable = TRUE) = 0;

		// 设置渲染状态
		virtual BOOL SetRenderState(D3DRENDERSTATETYPE enState, DWORD dwValue) = 0;

		// 重置设备
		virtual BOOL Reset() = 0;
		// 开始渲染
		virtual BOOL BeginRender(const D3DRECT* pRects = NULL, DWORD dwCount = 0, 
								DWORD dwFlags = D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
								D3DCOLOR dwColor = 0x00000000, float fZ = 1.0f, DWORD dwStencil = 0) = 0;
		// 结束渲染
		virtual BOOL EndRender() = 0;

	protected:
		// 构造函数
		ID3DDevice(LPDIRECT3DDEVICE9 pD3DDev9) 
			: ID3DBase(pD3DDev9)
		{
		}
	};


	// 网格接口
	class ID3DMesh : public ID3DBase
	{
	public:
		// 获取材质数量
		virtual DWORD GetMaterialNum() const = 0;
		// 获取材质
		virtual LPD3DXMATERIAL GetMaterial(DWORD dwId) = 0;

		// 绘制模型（子集）
		virtual BOOL DrawSubset(DWORD dwAttribId = 0) = 0;

	protected:
		// 构造函数
		ID3DMesh(LPD3DXMESH pD3DXMesh) 
			: ID3DBase(pD3DXMesh)
		{
		}
	};


	// 纹理接口
	class ID3DTexture : public ID3DBase
	{
	public:
		// 获取页面对象
		virtual LPDIRECT3DSURFACE9 GetSurface(UINT uLevel) = 0;
		// 获取描述信息
		virtual BOOL GetDesc(UINT uLevel, D3DSURFACE_DESC* pDesc) = 0;

	public:
		// 获取调色板
		virtual PPALETTEENTRY GetPalette() = 0;

		// 清空纹理页面数据
		virtual BOOL Clear(UINT uLevel, const RECT* pRect = NULL) = 0;

	public:
		// 加载页面数据
		//virtual BOOL LoadSurfaceFromFile(UINT uLevel, LPCTSTR pFile, //D3DXIMAGE_INFO* pImgInfo = NULL, 
		//						const RECT* pToRect = NULL, const RECT* pFromRect = NULL,
		//						D3DCOLOR dwColorKey = 0xFF000000, DWORD dwFilter = D3DX_DEFAULT) = 0;
		// 加载页面数据
		virtual BOOL LoadSurfaceFromMemory(UINT uLevel, LPCVOID pMemory, D3DFORMAT dwFormat, 
								const RECT* pRect, UINT uPitch, const PALETTEENTRY* pPalette = NULL, 
								const RECT* pStretchRect = NULL, D3DCOLOR dwColorKey = 0xFF000000, 
								DWORD dwFilter = D3DX_DEFAULT) = 0;
		// 加载页面数据
		virtual BOOL LoadSurfaceFromSurface(UINT uLevel, SP_ID3DTEXTURE spITex, 
								const RECT* pToRect = NULL, const RECT* pFromRect = NULL, 
								D3DCOLOR dwColorKey = 0xFF000000, DWORD dwFilter = D3DX_DEFAULT) = 0;

	protected:
		// 构造函数
		ID3DTexture(LPDIRECT3DTEXTURE9 pD3DTex9)
			: ID3DBase(pD3DTex9)
		{
		}

	};


	// 精灵接口
	class ID3DSprite : public ID3DBase
	{
	public:
		// 矩阵变换
		virtual BOOL SetTransform(const D3DXMATRIX* pMatrix) = 0;

		// 开始绘制
		virtual BOOL BeginDraw(DWORD dwFlags = 0) = 0;
		// 结束绘制
		virtual BOOL EndDraw() = 0;

		// 绘制
		virtual BOOL Draw(SP_ID3DTEXTURE spITex, 
						const D3DXVECTOR3* pPosition = NULL,
						const RECT* pSrcRect = NULL, 
						const D3DXVECTOR3* pCenter = NULL,
						D3DCOLOR dwColor = 0xFFFFFFFF) = 0;

	public:
		// 设备丢失事件
		virtual void OnLostDevice() = 0;
		// 设备重置事件
		virtual void OnResetDevice() = 0;

	protected:
		// 构造函数
		ID3DSprite(LPD3DXSPRITE pD3DXSprite)
			: ID3DBase(pD3DXSprite)
		{
		}
	};


	// 字体接口
	class ID3DFont : public ID3DBase
	{
	public:
		// 绘制文本
		virtual int DrawText(LPRECT pRect, LPCTSTR pString, INT nCount = -1, 
							DWORD dwFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE, 
							D3DCOLOR dwColor = 0xFFFFFFFF, ID3DSprite* pSprite = NULL) = 0;

	protected:
		// 构造函数
		ID3DFont(LPD3DXFONT pD3DXFont) 
			: ID3DBase(pD3DXFont)
		{
		}
	};


	/*****************************************************************************
	*
	*****************************************************************************/

	


}





#endif	// __MLT_DX_WRAP_H__