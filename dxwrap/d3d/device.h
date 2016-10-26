/******************************************************************************
*	�ļ���	:	device.h
*	��������	:	6/11/2016
*	����		:	Loser
*	����		:
******************************************************************************/

#ifndef	__MLT_D3D_DEVICE_H__
#define __MLT_D3D_DEVICE_H__

namespace mlt
{
	/**************************************************************************
	* ������
	**************************************************************************/

	class MD3DDevice : public ID3DDevice
	{
		friend class MDirect3D;

	public:
		// ����������
		virtual SP_ID3DMESH CreateMeshFVF(DWORD dwNumFaces, DWORD dwNumVertices, DWORD dwFVF,
											DWORD dwOptions = D3DXMESH_MANAGED);
		// ��.X�ļ���������
		virtual SP_ID3DMESH CreateMeshFromX(LPCTSTR pPath);

		// �������ӣ������壩����
		virtual SP_ID3DMESH CreateMeshBox(float fWidth, float fHeight, float fDepth);
		// ��������������
		virtual SP_ID3DMESH CreateMeshSphere(float fRadius, UINT uSlices, UINT uStacks);
		// ������������
		virtual SP_ID3DMESH CreateMeshCylinder(float fRadius1, float fRadius2, 
											float fLength, UINT uSlices, UINT uStacks);
		// �����������
		virtual SP_ID3DMESH CreateMeshTeapot();
		// �������������
		virtual SP_ID3DMESH CreateMeshPolygon(float fLength, UINT uSides);
		// ����Բ��������
		virtual SP_ID3DMESH CreateMeshTorus(float fInnerRadius, float fOuterRadius, 
											UINT uSides, UINT uRings); 


		// ��������
		virtual SP_ID3DTEXTURE CreateTexture(UINT uWidth, UINT uHeight, 
											D3DFORMAT dwFormat = D3DFMT_A8R8G8B8, 
											D3DPOOL dwPool = D3DPOOL_MANAGED, 
											DWORD dwUsage = 0, UINT uLevels = 0);
		// ���ļ���������
		virtual SP_ID3DTEXTURE CreateTextureFromFile(LPCTSTR pPath);

		// ��������
		virtual SP_ID3DSPRITE CreateSprite();

		// ��������
		virtual SP_ID3DFONT CreateFont(INT nHeight, UINT uWidth = 0, UINT uWeight = 0, 
									BOOL bItalic = FALSE, LPCTSTR pFaceName = TEXT("����"));

	public:
		// ��������ϵ���任
		virtual BOOL SetTransform(D3DTRANSFORMSTATETYPE enState, const D3DMATRIX* pMatrix);
		// �����ӿ�
		virtual BOOL SetViewport(DWORD dwX, DWORD dwY, DWORD dwWidth, DWORD dwHeight, 
									float fMinZ = 0.0f, float fMaxZ = 1.0f);

		// ���ò���
		virtual BOOL SetMaterial(D3DMATERIAL9* pD3DMtrl9);
		// ��������
		virtual BOOL SetTexture(DWORD dwStage, SP_ID3DTEXTURE spITex);
		
		// ���õƹ�
		virtual BOOL SetLight(DWORD dwIndex, const D3DLIGHT9* pLight);
		virtual BOOL LightEnable(DWORD dwLightIndex, BOOL bEnable = TRUE);
		
		// ������Ⱦ״̬
		virtual BOOL SetRenderState(D3DRENDERSTATETYPE enState, DWORD dwValue);

		// ����MDirect3D��Աs_PP�������豸
		virtual BOOL Reset();
		// ��ʼ��Ⱦ
		virtual BOOL BeginRender(const D3DRECT* pRects = NULL, DWORD dwCount = 0, 
								DWORD dwFlags = D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
								D3DCOLOR dwColor = 0x00000000, float fZ = 1.0f, DWORD dwStencil = 0);
		// ������Ⱦ
		virtual BOOL EndRender();

	protected:
		// ���캯��
		MD3DDevice(LPDIRECT3DDEVICE9 pD3DDev9);

	public:
		// �豸��ʧ�ź�
		static signal0<>	s_Lost;
		// �豸�����ź�
		static signal0<>	s_Reset;

	};



}




#endif	// __MLT_DX_DEVICE_H__






