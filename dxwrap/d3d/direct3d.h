/******************************************************************************
*	�ļ���	:	direct3d.h
*	��������	:	7/17/2010
*	����		:	Loser
*	����		:
******************************************************************************/

#ifndef	__MLT_DX_DIRECT3D_H__
#define __MLT_DX_DIRECT3D_H__

namespace mlt
{
	/**************************************************************************
	* ������
	**************************************************************************/

	DLL_CLASS MDirect3D
	{
	public:
		// ��ʼ��
		static BOOL Init(HWND hWnd, BOOL bWindowed = TRUE);
		// ����
		static void Finalize();

		// �����豸
		static SP_ID3DDEVICE CreateDevice();


	public:
		///////////////////////////////
		// ����任
		///////////////////////////////

		// ����ƽ��
		static D3DXMATRIX MatrixTranslation(float x, float y, float z);
		// ������X����ת
		static D3DXMATRIX MatrixRotationX(float fAngle);
		// ������Y����ת
		static D3DXMATRIX MatrixRotationY(float fAngle);
		// ������Z����ת
		static D3DXMATRIX MatrixRotationZ(float fAngle);
		// ��������
		static D3DXMATRIX MatrixScaling(float sx, float sy, float sz);

		// ��ͼ����任����
		static D3DXMATRIX MatrixLookAtLH(const D3DXVECTOR3* pEye = &D3DXVECTOR3(0.0f, 0.0f, -10.0f), 
										const D3DXVECTOR3* pAt = &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
										const D3DXVECTOR3* pUp = &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		// ͸��ͶӰ�任����
		static D3DXMATRIX MatrixPerspectiveFovLH(float fFovy = D3DX_PI * 0.25f, 
												float fAspect = (float)s_PP.BackBufferWidth / (float)s_PP.BackBufferHeight, 
												float fZn = 1.0f, float fZf = 1000.0f);


		///////////////////////////////
		// �����任
		///////////////////////////////

		// �任��
		static D3DXVECTOR3 Vec3TransformCoord(const D3DXVECTOR3* pV, const D3DXMATRIX* pM);
		// �任����
		static D3DXVECTOR3 Vec3TransformNormal(const D3DXVECTOR3* pV, const D3DXMATRIX* pM);
		// �任������
		static D3DXVECTOR3* Vec3TransformCoordArray(D3DXVECTOR3* pOut, UINT uOutStride, 
													const D3DXVECTOR3* pV, UINT uVStride, 
													const D3DXMATRIX* pM, UINT uN);
		// �任��������
		static D3DXVECTOR3* Vec3TransformNormalArray(D3DXVECTOR3* pOut, UINT uOutStride, 
													const D3DXVECTOR3* pV, UINT uVStride, 
													const D3DXMATRIX* pM, UINT uN);

		///////////////////////////////
		// ƽ��
		///////////////////////////////

		// �ж����ƽ���ϵ
		static float PlaneDotCoord(const D3DXPLANE* pP, const D3DXVECTOR3* pV);

		// ����ƽ�棨ָ�����ߺ͵㣩
		static D3DXPLANE PlaneFromPointNormal(const D3DXVECTOR3* pPoint, const D3DXVECTOR3* pNormal);
		// ����ƽ�棨ָ�������㣩
		static D3DXPLANE PlaneFromPoints(const D3DXVECTOR3* pV1, const D3DXVECTOR3* pV2, const D3DXVECTOR3* pV3);

		// ��׼��ƽ��
		D3DXPLANE PlaneNormalize(const D3DXPLANE* pP);
		// �任ƽ��
		D3DXPLANE PlaneTransform(const D3DXPLANE* pP, const D3DXMATRIX* pM);


	private:
		// ��ʼ���豸�ӿ�
		static void initDeviceView(LPDIRECT3DDEVICE9 pD3DDev9);
		// ��ʼ����Ⱦ״̬
		static void initRenderState(LPDIRECT3DDEVICE9 pD3DDev9);

	public:
		// �豸����
		static D3DCAPS9		s_Caps;
		// ��ǰ����
		static D3DPRESENT_PARAMETERS	s_PP;

	private:
		// D3D9ָ��
		static LPDIRECT3D9	s_pD3D9;
		
		
	};
}

#endif // __MLT_DX_DIRECT3D_H__











