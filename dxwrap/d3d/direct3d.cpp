/******************************************************************************
*	�ļ���	:	direct3d.cpp
*	��������	:	7/17/2010
*	����		:	Loser
*	����		:	
******************************************************************************/

#include "../dxwrap.h"
#include "direct3d.h"
#include "device.h"

namespace mlt
{
	// D3D9ָ��
	LPDIRECT3D9	MDirect3D::s_pD3D9 = NULL;

	// �豸����
	D3DCAPS9 MDirect3D::s_Caps = {(D3DDEVTYPE)0};
	// ��ǰ����
	D3DPRESENT_PARAMETERS MDirect3D::s_PP = {0};

	//-------------------------------------------------------------------------
	// ��ʼ��
	//-------------------------------------------------------------------------
	BOOL MDirect3D::Init(HWND hWnd, BOOL bWindowed)
	{
		//ASSERT(IsWindow(hWnd));
		if (!IsWindow(hWnd)) return FALSE;

		// ����D3D9����
		s_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
		if (!s_pD3D9)
		{
			MessageBox(hWnd, TEXT("����Direct3D����ʧ�ܣ�"), TEXT("����"), MB_OK | MB_ICONERROR);
			return FALSE;
		}

		// ��ȡ�豸����
		HRESULT hr = s_pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &s_Caps);
		if (FAILED(hr))
		{
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return FALSE;
		}

		// 
		RECT rcClient;
		GetClientRect(hWnd, &rcClient);

		// ���D3DPRESENT_PARAMETERS�ṹ
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
	// ����
	//-------------------------------------------------------------------------
	void MDirect3D::Finalize()
	{
		// �ͷ�D3D����
		SAFE_RELEASE(s_pD3D9);

		// ��ճ�Ա����
		ZeroMemory(&s_Caps, sizeof(s_Caps));
		ZeroMemory(&s_PP, sizeof(s_PP));
	}

	//-------------------------------------------------------------------------
	// �����豸
	//-------------------------------------------------------------------------
	SP_ID3DDEVICE MDirect3D::CreateDevice()
	{
		//ASSERT(NULL != s_pD3D9);
		if (!s_pD3D9) return NULL;

		// 
		LPDIRECT3DDEVICE9 pD3DDev9 = NULL;

		// ����IDirect3DDevice9����
		HRESULT hr = s_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
										s_PP.hDeviceWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING,
										&s_PP, &pD3DDev9);
		if (FAILED(hr))
		{
			// ʧ�ܴ���
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			return NULL;
		}
		
		// �����Դ�
		pD3DDev9->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0,0,0,0), 0.0f, 0);
		pD3DDev9->EvictManagedResources();

		// ��ʼ���豸�ӿ�
		initDeviceView(pD3DDev9);
		// ��ʼ����Ⱦ״̬
		initRenderState(pD3DDev9);

		return SP_ID3DDEVICE(new MD3DDevice(pD3DDev9));
	}


	//-------------------------------------------------------------------------
	// ����ƽ��
	//-------------------------------------------------------------------------
	D3DXMATRIX MDirect3D::MatrixTranslation(float x, float y, float z)
	{
		D3DXMATRIX mtrx;

		return *D3DXMatrixTranslation(&mtrx, x, y, z);
	}

	//-------------------------------------------------------------------------
	// ������X����ת
	//-------------------------------------------------------------------------
	D3DXMATRIX MDirect3D::MatrixRotationX(float fAngle)
	{
		D3DXMATRIX mtrx;

		return *D3DXMatrixRotationX(&mtrx, fAngle);
	}

	//-------------------------------------------------------------------------
	// ������Y����ת
	//-------------------------------------------------------------------------
	D3DXMATRIX MDirect3D::MatrixRotationY(float fAngle)
	{
		D3DXMATRIX mtrx;

		return *D3DXMatrixRotationY(&mtrx, fAngle);
	}

	//-------------------------------------------------------------------------
	// ������Z����ת
	//-------------------------------------------------------------------------
	D3DXMATRIX MDirect3D::MatrixRotationZ(float fAngle)
	{
		D3DXMATRIX mtrx;

		return *D3DXMatrixRotationZ(&mtrx, fAngle);
	}

	//-------------------------------------------------------------------------
	// ��������
	//-------------------------------------------------------------------------
	D3DXMATRIX MDirect3D::MatrixScaling(float sx, float sy, float sz)
	{
		D3DXMATRIX mtrx;

		return *D3DXMatrixScaling(&mtrx, sx, sy, sz);
	}

	//-------------------------------------------------------------------------
	// ��ͼ����任����
	//-------------------------------------------------------------------------
	D3DXMATRIX MDirect3D::MatrixLookAtLH(const D3DXVECTOR3* pEye /*= &D3DXVECTOR3(0.0f, 0.0f, -10.0f)*/, 
										const D3DXVECTOR3* pAt /*= &D3DXVECTOR3(0.0f, 0.0f, 0.0f)*/, 
										const D3DXVECTOR3* pUp /*= &D3DXVECTOR3(0.0f, 1.0f, 0.0f)*/)
	{
		D3DXMATRIX mtrx;

		return *D3DXMatrixLookAtLH(&mtrx, pEye, pAt, pUp);
	}

	//-------------------------------------------------------------------------
	// ͸��ͶӰ�任����
	//-------------------------------------------------------------------------
	D3DXMATRIX MDirect3D::MatrixPerspectiveFovLH(float fFovy /*= D3DX_PI * 0.25f*/, 
												float fAspect /*= (float)s_PP.BackBufferWidth / (float)s_PP.BackBufferHeight*/, 
												float fZn /*= 1.0f*/, float fZf /*= 1000.0f*/)
	{
		D3DXMATRIX mtrx;

		return *D3DXMatrixPerspectiveFovLH(&mtrx, fFovy, fAspect, fZn, fZf);
	}


	//-------------------------------------------------------------------------
	// ��ʼ���豸�ӿ�
	//-------------------------------------------------------------------------
	void MDirect3D::initDeviceView(LPDIRECT3DDEVICE9 pD3DDev9)
	{
		//ASSERT(NULL != pD3DDev9);
		if (!pD3DDev9) return ;

		// ��ͼ����ϵ
		pD3DDev9->SetTransform(D3DTS_VIEW, 
						&MatrixLookAtLH(&D3DXVECTOR3(0.0f, 0.0f, -10.0f), 
										&D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
										&D3DXVECTOR3(0.0f, 1.0f, 0.0f)));

		// ͶӰ�任�����������ӽ�Ϊ120�ȣ�
		pD3DDev9->SetTransform(D3DTS_PROJECTION, 
						&MatrixPerspectiveFovLH(D3DX_PI / 4.0, 
						(float)s_PP.BackBufferWidth / (float)s_PP.BackBufferHeight,
						1.0f, 1000.0f));

		// �ӿڱ任
		D3DVIEWPORT9 vp9 = { 0, 0, s_PP.BackBufferWidth, s_PP.BackBufferHeight, 0.0f, 1.0f };
		pD3DDev9->SetViewport(&vp9);
	}

	// ��ʼ����Ⱦ״̬
	void MDirect3D::initRenderState(LPDIRECT3DDEVICE9 pD3DDev9)
	{
		//ASSERT(NULL != pD3DDev9);
		if (!pD3DDev9) return ;

		// ����ͨ�ò���
		D3DMATERIAL9 mtrl;
		mtrl.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		mtrl.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		mtrl.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		mtrl.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		mtrl.Power = 5.0f;
		pD3DDev9->SetMaterial(&mtrl);

		// ���ù���
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

		// �������
		//pD3DDev9->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
		//pD3DDev9->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
		//pD3DDev9->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);

		//pD3DDev9->SetRenderState(D3DRS_LIGHTING, FALSE);				// �رչ���
		//pD3DDev9->SetRenderState(D3DRS_ZENABLE, FALSE);				// Z��
		//pD3DDev9->SetRenderState(D3DRS_STENCILENABLE, FALSE);			// ģ��

		pD3DDev9->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			// �����ѡ
		pD3DDev9->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);			// ���ģʽ
		//pD3DDev9->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);	// ��ɫģʽ

		//pD3DDev9->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); 
		//pD3DDev9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		//pD3DDev9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//pD3DDev9->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		//pD3DDev9->SetRenderState(D3DRS_ALPHAREF, 0x01);
		//pD3DDev9->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}
	
}











