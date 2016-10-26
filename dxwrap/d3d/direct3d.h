/******************************************************************************
*	文件名	:	direct3d.h
*	创建日期	:	7/17/2010
*	作者		:	Loser
*	描述		:
******************************************************************************/

#ifndef	__MLT_DX_DIRECT3D_H__
#define __MLT_DX_DIRECT3D_H__

namespace mlt
{
	/**************************************************************************
	* 类声明
	**************************************************************************/

	DLL_CLASS MDirect3D
	{
	public:
		// 初始化
		static BOOL Init(HWND hWnd, BOOL bWindowed = TRUE);
		// 结束
		static void Finalize();

		// 创建设备
		static SP_ID3DDEVICE CreateDevice();


	public:
		///////////////////////////////
		// 矩阵变换
		///////////////////////////////

		// 矩阵平移
		static D3DXMATRIX MatrixTranslation(float x, float y, float z);
		// 矩阵绕X轴旋转
		static D3DXMATRIX MatrixRotationX(float fAngle);
		// 矩阵绕Y轴旋转
		static D3DXMATRIX MatrixRotationY(float fAngle);
		// 矩阵绕Z轴旋转
		static D3DXMATRIX MatrixRotationZ(float fAngle);
		// 矩阵缩放
		static D3DXMATRIX MatrixScaling(float sx, float sy, float sz);

		// 视图坐标变换矩阵
		static D3DXMATRIX MatrixLookAtLH(const D3DXVECTOR3* pEye = &D3DXVECTOR3(0.0f, 0.0f, -10.0f), 
										const D3DXVECTOR3* pAt = &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
										const D3DXVECTOR3* pUp = &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		// 透视投影变换矩阵
		static D3DXMATRIX MatrixPerspectiveFovLH(float fFovy = D3DX_PI * 0.25f, 
												float fAspect = (float)s_PP.BackBufferWidth / (float)s_PP.BackBufferHeight, 
												float fZn = 1.0f, float fZf = 1000.0f);


		///////////////////////////////
		// 向量变换
		///////////////////////////////

		// 变换点
		static D3DXVECTOR3 Vec3TransformCoord(const D3DXVECTOR3* pV, const D3DXMATRIX* pM);
		// 变换向量
		static D3DXVECTOR3 Vec3TransformNormal(const D3DXVECTOR3* pV, const D3DXMATRIX* pM);
		// 变换点数组
		static D3DXVECTOR3* Vec3TransformCoordArray(D3DXVECTOR3* pOut, UINT uOutStride, 
													const D3DXVECTOR3* pV, UINT uVStride, 
													const D3DXMATRIX* pM, UINT uN);
		// 变换向量数组
		static D3DXVECTOR3* Vec3TransformNormalArray(D3DXVECTOR3* pOut, UINT uOutStride, 
													const D3DXVECTOR3* pV, UINT uVStride, 
													const D3DXMATRIX* pM, UINT uN);

		///////////////////////////////
		// 平面
		///////////////////////////////

		// 判定点和平面关系
		static float PlaneDotCoord(const D3DXPLANE* pP, const D3DXVECTOR3* pV);

		// 创建平面（指定法线和点）
		static D3DXPLANE PlaneFromPointNormal(const D3DXVECTOR3* pPoint, const D3DXVECTOR3* pNormal);
		// 创建平面（指定三个点）
		static D3DXPLANE PlaneFromPoints(const D3DXVECTOR3* pV1, const D3DXVECTOR3* pV2, const D3DXVECTOR3* pV3);

		// 标准化平面
		D3DXPLANE PlaneNormalize(const D3DXPLANE* pP);
		// 变换平面
		D3DXPLANE PlaneTransform(const D3DXPLANE* pP, const D3DXMATRIX* pM);


	private:
		// 初始化设备视口
		static void initDeviceView(LPDIRECT3DDEVICE9 pD3DDev9);
		// 初始化渲染状态
		static void initRenderState(LPDIRECT3DDEVICE9 pD3DDev9);

	public:
		// 设备性能
		static D3DCAPS9		s_Caps;
		// 当前参数
		static D3DPRESENT_PARAMETERS	s_PP;

	private:
		// D3D9指针
		static LPDIRECT3D9	s_pD3D9;
		
		
	};
}

#endif // __MLT_DX_DIRECT3D_H__











