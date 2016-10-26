/******************************************************************************
*	文件名	:	sprite.h
*	创建日期	:	7/2/2016
*	作者		:	Loser
*	描述		:	
******************************************************************************/

#ifndef __MLT_D3D_SPRITE_H__
#define __MLT_D3D_SPRITE_H__

namespace mlt
{
	/**************************************************************************
	* 类声明
	**************************************************************************/

	class MD3DSprite : public ID3DSprite, public has_slots<>
	{
		friend class MD3DDevice;

	public:
		// 虚析构函数
		virtual ~MD3DSprite();

		// 矩阵变换
		virtual BOOL SetTransform(const D3DXMATRIX* pMatrix);

		// 开始绘制
		virtual BOOL BeginDraw(DWORD dwFlags = 0);
		// 结束绘制
		virtual BOOL EndDraw();

		// 绘制
		virtual BOOL Draw(SP_ID3DTEXTURE spITex, 
						const D3DXVECTOR3* pPosition = NULL,
						const RECT* pSrcRect = NULL, 
						const D3DXVECTOR3* pCenter = NULL,
						D3DCOLOR dwColor = 0xFFFFFFFF);

	public:
		// 设备丢失事件
		virtual void OnLostDevice();
		// 设备重置事件
		virtual void OnResetDevice();

	protected:
		// 构造函数
		MD3DSprite(LPD3DXSPRITE pD3DXSprite);
		
		

	};
}


#endif // __MLT_D3D_SPRITE_H__