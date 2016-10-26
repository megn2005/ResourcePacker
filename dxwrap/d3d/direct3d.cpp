/******************************************************************************
*	文件名	:	direct3d.cpp
*	创建日期	:	7/17/2010
*	作者		:	Loser
*	描述		:	
******************************************************************************/

#include "../dxwrap.h"
#include "direct3d.h"
#include "device.h"

namespace mlt
{
	// D3D9指针
	LPDIRECT3D9	MDirect3D::s_pD3D9 = NULL;

	// 设备性能
	D3DCAPS9 MDirect3D::s_Caps = {(D3DDEVTYPE)0};
	// 当前参数
	D3DPRESENT_PARAMETERS MDirect3D::s_PP = {0};

	//-------------------------------------------------------------------------
	// 初始化
	//-------------------------------------------------------------------------
	BOOL MDirect3D::Init(HWND hWnd, BOOL bWindowed)
	{
		//ASSERT(IsWindow(hWnd));
		if (!IsWindow(hWnd)) return FALSE;

		// 创建D3D9对象
		s_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
		if (!s_pD3D9)
		{
			MessageBox(hWnd, TEXT("创建Direct3D对象失败！"), TEXT("错误"), MB_OK | MB_ICONERROR);
			return FALSE;
		}

		// 获取设备性能
		HRESULT hr = s_pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &s_Caps);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		// 
		RECT rcClient;
		GetClientRect(hWnd, &rcClient);

		// 填充D3DPRESENT_PARAMETERS结构
		s_PP.BackBufferWidth			= rcClient.right - rcClient.left;
		s_PP.BackBufferHeight			= rcClient.bottom - rcClient.top;
		s_PP.BackBufferFormat			= D3DFMT_A8R8G8B8;
		s_PP.BackBufferCount			= 2;
		s_PP.MultiSampleType			= D3DMULTISAMPLE_NONE;
		s_PP.MultiSampleQuality			= 0;
		s_PP.SwapEffect					= D3DSWAPEFFECT_DISCARD;
		s_PP.hDeviceWindow				= hWnd;
		s_PP.Windowed					= bWindowed;
		s_PP.EnableAutoDepthStencil		= TRUE; 
		s_PP.AutoDepthStencilFormat		= D3DFMT_D24S8;//D3DFMT_D15S1;
		s_PP.Flags						= 0;//D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
		s_PP.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
		s_PP.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;

		return TRUE;
	}

	//-------------------------------------------------------------------------
	// 结束
	//-------------------------------------------------------------------------
	void MDirect3D::Finalize()
	{
		// 释放D3D对象
		SAFE_RELEASE(s_pD3D9);

		// 清空成员变量
		ZeroMemory(&s_Caps, sizeof(s_Caps));
		ZeroMemory(&s_PP, sizeof(s_PP));
	}

	//-------------------------------------------------------------------------
	// 创建设备
	//-------------------------------------------------------------------------
	SP_ID3DDEVICE MDirect3D::CreateDevice()
	{
		//ASSERT(NULL != s_pD3D9);
		if (!s_pD3D9) return NULL;

		// 
		LPDIRECT3DDEVICE9 pD3DDev9 = NULL;

		// 创建IDirect3DDevice9对象
		HRESULT hr = s_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
										s_PP.hDeviceWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING,
										&s_PP, &pD3DDev9);
		if (FAILED(hr))
		{
			// 失败处理
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return NULL;
		}
		
		// 清理显存
		pD3DDev9->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0,0,0,0), 0.0f, 0);
		pD3DDev9->EvictManagedResources();

		// 初始化设备视口
		initDeviceView(pD3DDev9);
		// 初始化渲染状态
		initRenderState(pD3DDev9);

		return SP_ID3DDEVICE(new MD3DDevice(pD3DDev9));
	}


	//-------------------------------------------------------------------------
	// 矩阵平移
	//-------------------------------------------------------------------------
	D3DXMATRIX MDirect3D::MatrixTranslation(float x, float y, float z)
	{
		D3DXMATRIX mtrx;

		return *D3DXMatrixTranslation(&mtrx, x, y, z);
	}

	//-------------------------------------------------------------------------
	// 矩阵绕X轴旋转
	//-------------------------------------------------------------------------
	D3DXMATRIX MDirect3D::MatrixRotationX(float fAngle)
	{
		D3DXMATRIX mtrx;

		return *D3DXMatrixRotationX(&mtrx, fAngle);
	}

	//-------------------------------------------------------------------------
	// 矩阵绕Y轴旋转
	//-------------------------------------------------------------------------
	D3DXMATRIX MDirect3D::MatrixRotationY(float fAngle)
	{
		D3DXMATRIX mtrx;

		return *D3DXMatrixRotationY(&mtrx, fAngle);
	}

	//-------------------------------------------------------------------------
	// 矩阵绕Z轴旋转
	//-------------------------------------------------------------------------
	D3DXMATRIX MDirect3D::MatrixRotationZ(float fAngle)
	{
		D3DXMATRIX mtrx;

		return *D3DXMatrixRotationZ(&mtrx, fAngle);
	}

	//-------------------------------------------------------------------------
	// 矩阵缩放
	//-------------------------------------------------------------------------
	D3DXMATRIX MDirect3D::MatrixScaling(float sx, float sy, float sz)
	{
		D3DXMATRIX mtrx;

		return *D3DXMatrixScaling(&mtrx, sx, sy, sz);
	}

	//-------------------------------------------------------------------------
	// 视图坐标变换矩阵
	//-------------------------------------------------------------------------
	D3DXMATRIX MDirect3D::MatrixLookAtLH(const D3DXVECTOR3* pEye /*= &D3DXVECTOR3(0.0f, 0.0f, -10.0f)*/, 
										const D3DXVECTOR3* pAt /*= &D3DXVECTOR3(0.0f, 0.0f, 0.0f)*/, 
										const D3DXVECTOR3* pUp /*= &D3DXVECTOR3(0.0f, 1.0f, 0.0f)*/)
	{
		D3DXMATRIX mtrx;

		return *D3DXMatrixLookAtLH(&mtrx, pEye, pAt, pUp);
	}

	//-------------------------------------------------------------------------
	// 透视投影变换矩阵
	//-------------------------------------------------------------------------
	D3DXMATRIX MDirect3D::MatrixPerspectiveFovLH(float fFovy /*= D3DX_PI * 0.25f*/, 
												float fAspect /*= (float)s_PP.BackBufferWidth / (float)s_PP.BackBufferHeight*/, 
												float fZn /*= 1.0f*/, float fZf /*= 1000.0f*/)
	{
		D3DXMATRIX mtrx;

		return *D3DXMatrixPerspectiveFovLH(&mtrx, fFovy, fAspect, fZn, fZf);
	}


	//-------------------------------------------------------------------------
	// 初始化设备视口
	//-------------------------------------------------------------------------
	void MDirect3D::initDeviceView(LPDIRECT3DDEVICE9 pD3DDev9)
	{
		//ASSERT(NULL != pD3DDev9);
		if (!pD3DDev9) return ;

		// 视图坐标系
		pD3DDev9->SetTransform(D3DTS_VIEW, 
						&MatrixLookAtLH(&D3DXVECTOR3(0.0f, 0.0f, -10.0f), 
										&D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
										&D3DXVECTOR3(0.0f, 1.0f, 0.0f)));

		// 投影变换（人眼左右视角为120度）
		pD3DDev9->SetTransform(D3DTS_PROJECTION, 
						&MatrixPerspectiveFovLH(D3DX_PI / 4.0, 
						(float)s_PP.BackBufferWidth / (float)s_PP.BackBufferHeight,
						1.0f, 1000.0f));

		// 视口变换
		D3DVIEWPORT9 vp9 = { 0, 0, s_PP.BackBufferWidth, s_PP.BackBufferHeight, 0.0f, 1.0f };
		pD3DDev9->SetViewport(&vp9);
	}

	// 初始化渲染状态
	void MDirect3D::initRenderState(LPDIRECT3DDEVICE9 pD3DDev9)
	{
		//ASSERT(NULL != pD3DDev9);
		if (!pD3DDev9) return ;

		// 设置通用材质
		D3DMATERIAL9 mtrl;
		mtrl.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		mtrl.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		mtrl.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		mtrl.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		mtrl.Power = 5.0f;
		pD3DDev9->SetMaterial(&mtrl);

		// 设置光照
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(light));
		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		light.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		light.Direction = D3DXVECTOR3(1.0f, 0.0f, 1.0f);

		pD3DDev9->SetLight(0, &light);
		pD3DDev9->LightEnable(0, TRUE);
		pD3DDev9->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		//pD3DDev9->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

		// 纹理过滤
		//pD3DDev9->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
		//pD3DDev9->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
		//pD3DDev9->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);

		//pD3DDev9->SetRenderState(D3DRS_LIGHTING, FALSE);				// 关闭光照
		//pD3DDev9->SetRenderState(D3DRS_ZENABLE, FALSE);				// Z轴
		//pD3DDev9->SetRenderState(D3DRS_STENCILENABLE, FALSE);			// 模板

		pD3DDev9->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			// 背面拣选
		pD3DDev9->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);			// 填充模式
		//pD3DDev9->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);	// 着色模式

		//pD3DDev9->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); 
		//pD3DDev9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		//pD3DDev9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//pD3DDev9->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		//pD3DDev9->SetRenderState(D3DRS_ALPHAREF, 0x01);
		//pD3DDev9->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}
	
}











