/******************************************************************************
*	�ļ���:		dxwrap.h
*	��������:		6/17/2016
*	����:		Loser
*	����:		
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

// �źŲ�
#include "sigslot.h"
using namespace sigslot;

namespace mlt
{
	/**************************************************************************
	* 
	**************************************************************************/

	// ���뵼��
	#ifdef	DXWRAP_EXPORTS
		#define	DLL_CLASS	class __declspec(dllexport)
		#define	DLL_FUNC	__declspec(dllexport)
	#else
		#define	DLL_CLASS	class __declspec(dllimport)
		#define	DLL_FUNC	__declspec(dllimport)
	#endif

	// ��ȫ�ͷ�
	#define SAFE_RELEASE(ptr)	if (ptr) { ptr->Release(); ptr = NULL; }


	/*****************************************************************************
	*
	*****************************************************************************/

	// �ӿ�����
	class ID3DTexture;
	class ID3DDevice;
	class ID3DMesh;
	class ID3DTexture;
	class ID3DSprite;
	class ID3DFont;

	// ����ָ������
	typedef shared_ptr<ID3DDevice>		SP_ID3DDEVICE;
	typedef shared_ptr<ID3DMesh>		SP_ID3DMESH;
	typedef shared_ptr<ID3DTexture>		SP_ID3DTEXTURE;
	typedef shared_ptr<ID3DSprite>		SP_ID3DSPRITE;
	typedef shared_ptr<ID3DFont>		SP_ID3DFONT;
	
	
	/**************************************************************************
	* �ӿڻ���
	**************************************************************************/

	class ID3DBase
	{
	public:
		// ���캯��
		ID3DBase(LPUNKNOWN pIUnknown) 
			: m_pIUnknown(pIUnknown)
		{
		}

		// ����������
		virtual ~ID3DBase() 
		{ 
			Release(); 
		}

		// �ͷŶ���
		virtual void Release() 
		{ 
			SAFE_RELEASE(m_pIUnknown); 
		}

		// ��ȡ�ӿ�ָ��
		virtual LPUNKNOWN GetInterface() 
		{ 
			return m_pIUnknown; 
		} 

	protected:
		// �ӿ�ָ��
		LPUNKNOWN	m_pIUnknown;
	};


	/**************************************************************************
	* �ӿ�����
	**************************************************************************/

	// �豸�ӿ�
	class ID3DDevice : public ID3DBase
	{
	public:
		// ��������
		virtual SP_ID3DMESH CreateMesh() = 0;
		// ��.X�ļ���������
		virtual SP_ID3DMESH CreateMeshFromX(LPCTSTR pPath) = 0;

		// �������ӣ������壩����
		virtual SP_ID3DMESH CreateMeshBox(FLOAT fWidth, FLOAT fHeight, FLOAT fDepth) = 0;
		// ��������������
		virtual SP_ID3DMESH CreateMeshSphere(FLOAT fRadius, UINT uSlices, UINT uStacks) = 0;
		// ������������
		virtual SP_ID3DMESH CreateMeshCylinder(FLOAT fRadius1, FLOAT fRadius2, 
											FLOAT fLength, UINT uSlices, UINT uStacks) = 0;
		// �����������
		virtual SP_ID3DMESH CreateMeshTeapot() = 0;
		// �������������
		virtual SP_ID3DMESH CreateMeshPolygon(FLOAT fLength, UINT uSides) = 0;
		// ����Բ��������
		virtual SP_ID3DMESH CreateMeshTorus(FLOAT fInnerRadius, FLOAT fOuterRadius, 
											UINT uSides, UINT uRings) = 0; 

		// ��������
		virtual SP_ID3DTEXTURE CreateTexture(UINT uWidth, UINT uHeight, 
											D3DFORMAT dwFormat = D3DFMT_A8R8G8B8, 
											D3DPOOL dwPool = D3DPOOL_MANAGED, 
											DWORD dwUsage = 0, UINT uLevels = 0) = 0;
		// ���ļ���������
		virtual SP_ID3DTEXTURE CreateTextureFromFile(LPCTSTR pPath) = 0;

		// ��������
		virtual SP_ID3DSPRITE CreateSprite() = 0;

		// ��������
		virtual SP_ID3DFONT CreateFont(INT nHeight, UINT uWidth = 0, UINT uWeight = 0, 
									BOOL bItalic = FALSE, LPCTSTR pFaceName = TEXT("����")) = 0;

	public:
		// ��������ϵ���任
		virtual BOOL SetTransform(D3DTRANSFORMSTATETYPE enState, const D3DMATRIX* pMatrix) = 0;
		// �����ӿ�
		virtual BOOL SetViewport(DWORD dwX, DWORD dwY, DWORD dwWidth, DWORD dwHeight, 
									float fMinZ = 0.0f, float fMaxZ = 1.0f) = 0;

		// ���ò���
		virtual BOOL SetMaterial(D3DMATERIAL9* pD3DMtrl9) = 0;
		// ��������
		virtual BOOL SetTexture(DWORD dwStage, SP_ID3DTEXTURE spITex) = 0;

		// ���õƹ�
		virtual BOOL SetLight(DWORD dwIndex, const D3DLIGHT9* pLight) = 0;
		virtual BOOL LightEnable(DWORD dwLightIndex, BOOL bEnable = TRUE) = 0;

		// ������Ⱦ״̬
		virtual BOOL SetRenderState(D3DRENDERSTATETYPE enState, DWORD dwValue) = 0;

		// �����豸
		virtual BOOL Reset() = 0;
		// ��ʼ��Ⱦ
		virtual BOOL BeginRender(const D3DRECT* pRects = NULL, DWORD dwCount = 0, 
								DWORD dwFlags = D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
								D3DCOLOR dwColor = 0x00000000, float fZ = 1.0f, DWORD dwStencil = 0) = 0;
		// ������Ⱦ
		virtual BOOL EndRender() = 0;

	protected:
		// ���캯��
		ID3DDevice(LPDIRECT3DDEVICE9 pD3DDev9) 
			: ID3DBase(pD3DDev9)
		{
		}
	};


	// ����ӿ�
	class ID3DMesh : public ID3DBase
	{
	public:
		// ��ȡ��������
		virtual DWORD GetMaterialNum() const = 0;
		// ��ȡ����
		virtual LPD3DXMATERIAL GetMaterial(DWORD dwId) = 0;

		// ����ģ�ͣ��Ӽ���
		virtual BOOL DrawSubset(DWORD dwAttribId = 0) = 0;

	protected:
		// ���캯��
		ID3DMesh(LPD3DXMESH pD3DXMesh) 
			: ID3DBase(pD3DXMesh)
		{
		}
	};


	// ����ӿ�
	class ID3DTexture : public ID3DBase
	{
	public:
		// ��ȡҳ�����
		virtual LPDIRECT3DSURFACE9 GetSurface(UINT uLevel) = 0;
		// ��ȡ������Ϣ
		virtual BOOL GetDesc(UINT uLevel, D3DSURFACE_DESC* pDesc) = 0;

	public:
		// ��ȡ��ɫ��
		virtual PPALETTEENTRY GetPalette() = 0;

		// �������ҳ������
		virtual BOOL Clear(UINT uLevel, const RECT* pRect = NULL) = 0;

	public:
		// ����ҳ������
		//virtual BOOL LoadSurfaceFromFile(UINT uLevel, LPCTSTR pFile, //D3DXIMAGE_INFO* pImgInfo = NULL, 
		//						const RECT* pToRect = NULL, const RECT* pFromRect = NULL,
		//						D3DCOLOR dwColorKey = 0xFF000000, DWORD dwFilter = D3DX_DEFAULT) = 0;
		// ����ҳ������
		virtual BOOL LoadSurfaceFromMemory(UINT uLevel, LPCVOID pMemory, D3DFORMAT dwFormat, 
								const RECT* pRect, UINT uPitch, const PALETTEENTRY* pPalette = NULL, 
								const RECT* pStretchRect = NULL, D3DCOLOR dwColorKey = 0xFF000000, 
								DWORD dwFilter = D3DX_DEFAULT) = 0;
		// ����ҳ������
		virtual BOOL LoadSurfaceFromSurface(UINT uLevel, SP_ID3DTEXTURE spITex, 
								const RECT* pToRect = NULL, const RECT* pFromRect = NULL, 
								D3DCOLOR dwColorKey = 0xFF000000, DWORD dwFilter = D3DX_DEFAULT) = 0;

	protected:
		// ���캯��
		ID3DTexture(LPDIRECT3DTEXTURE9 pD3DTex9)
			: ID3DBase(pD3DTex9)
		{
		}

	};


	// ����ӿ�
	class ID3DSprite : public ID3DBase
	{
	public:
		// ����任
		virtual BOOL SetTransform(const D3DXMATRIX* pMatrix) = 0;

		// ��ʼ����
		virtual BOOL BeginDraw(DWORD dwFlags = 0) = 0;
		// ��������
		virtual BOOL EndDraw() = 0;

		// ����
		virtual BOOL Draw(SP_ID3DTEXTURE spITex, 
						const D3DXVECTOR3* pPosition = NULL,
						const RECT* pSrcRect = NULL, 
						const D3DXVECTOR3* pCenter = NULL,
						D3DCOLOR dwColor = 0xFFFFFFFF) = 0;

	public:
		// �豸��ʧ�¼�
		virtual void OnLostDevice() = 0;
		// �豸�����¼�
		virtual void OnResetDevice() = 0;

	protected:
		// ���캯��
		ID3DSprite(LPD3DXSPRITE pD3DXSprite)
			: ID3DBase(pD3DXSprite)
		{
		}
	};


	// ����ӿ�
	class ID3DFont : public ID3DBase
	{
	public:
		// �����ı�
		virtual int DrawText(LPRECT pRect, LPCTSTR pString, INT nCount = -1, 
							DWORD dwFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE, 
							D3DCOLOR dwColor = 0xFFFFFFFF, ID3DSprite* pSprite = NULL) = 0;

	protected:
		// ���캯��
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