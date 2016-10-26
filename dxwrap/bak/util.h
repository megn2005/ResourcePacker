/******************************************************************************
*	filename:	utility.h
*	created:	7/11/2010
*	author:		Loser
*		
*	purpose:	工具函数集.
******************************************************************************/

#ifndef __MLT_DX_UTIL_H__
#define __MLT_DX_UTIL_H__

namespace mlt
{
	/**************************************************************************
	* 纹理混合 
	**************************************************************************/

	// 多纹理混合
	#define	SetTextureColorStage(dev, i, arg1, op, arg2)				\
			{															\
				dev->SetTextureStageState( i, D3DTSS_COLORARG1, arg1 );	\
				dev->SetTextureStageState( i, D3DTSS_COLORARG2, arg2 );	\
				dev->SetTextureStageState( i, D3DTSS_COLOROP, op );		\
			}

	#define	SetTextureAlphaStage( dev, i, arg1, op, arg2 )				\
			{															\
				dev->SetTextureStageState( i, D3DTSS_ALPHAARG1, arg1 );	\
				dev->SetTextureStageState( i, D3DTSS_ALPHAARG2, arg2 );	\
				dev->SetTextureStageState( i, D3DTSS_ALPHAOP, op );		\
			}

	// 渲染状态混合
	#define	SetRenderAlphaBlend( dev, src_mode, op, dest_mode )			\
			{															\
				dev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );		\
				dev->SetRenderState( D3DRS_SRCBLEND, src_mode );			\
				dev->SetRenderState( D3DRS_DESTBLEND, dest_mode );		\
				dev->SetRenderState( D3DRS_BLENDOP, op );				\
			}

	#define SetSeparateAlphaBlend( dev, src_mode, op, dest_mode )				\
			{																	\
				dev->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );		\
				dev->SetRenderState( D3DRS_SRCBLENDALPHA, src_mode );			\
				dev->SetRenderState( D3DRS_DESTBLENDALPHA, dest_mode );			\
				dev->SetRenderState( D3DRS_BLENDOPALPHA, op );					\
			}

	// alpha test
	#define	SetRenderAlphaTest( dev, ref, func )					\
			{														\
				dev->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );	\
				dev->SetRenderState( D3DRS_ALPHAREF, ref );			\
				dev->SetRenderState( D3DRS_ALPHAFUNC, func );		\
			}


	/**************************************************************************
	* 混合模式
	**************************************************************************/

	// 测试方式
	enum _test_mode
	{
		tmAlpha = 1,	// alpha test
		tmMax = 8
	};

	// 透明方式
	enum _transp_mode
	{
		tpAlpha = tmMax + 1,	// alpha透明
		tpGlass,				// 玻璃透明
		tpFactor,				// alpha factor透明

		tpMax = 16
	};

	// 加亮方式
	enum _light_mode
	{
		ltColorAdd = tpMax + 1,	// 颜色累加
		ltAddLight,				// 加亮
		ltColorMul,				// 高亮
		ltMulLight,				// 高光

		ltMax = 32
	};

	// 混合方式
	enum _blend_mode
	{
		blColorUp = ltMax + 1,	// 上色
		blMax = 64
	};

	// 盛大方式
	enum _shengda_mode
	{
		sdFxNone = 128,
		sdFxAdd,
		sdFxAddX,
		sdFxSub,
		sdFxLight,
		sdFxBright,
		sdFxBlend,
		sdFxBlendNA,
		sdFxBlend1X,
		sdFxBlend2X,
		sdFxMultiply,
		sdFxMultiplyAlpha,
		sdFxMax,
		sdFxMin,
		sdFxSrcColorAdd,
		sdFxInvert,
		sdFxSrcBright,
		sdFxDestBright,
		sdFxAddSmooth,
	};

	// alpha factor 透明
	#define	SetTextureTransprt( dev, degree )									\
			{																	\
				dev->SetRenderState( D3DRS_TEXTUREFACTOR, degree );				\
				dev->SetTextureStageState( 0,D3DTSS_ALPHAARG1,D3DTA_TFACTOR );	\
				dev->SetTextureStageState( 0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1 );\
				SetRenderAlphaBlend( dev, D3DBLEND_SRCALPHA, D3DBLENDOP_ADD, D3DBLEND_INVSRCALPHA );\
			}

	// 上色
	#define	SetTextureColorUp( dev, factor )								\
			{																\
				dev->SetRenderState( D3DRS_TEXTUREFACTOR, factor );			\
				dev->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0 );		\
				SetTextureColorStage( dev, 1, D3DTA_TFACTOR, D3DTOP_BLENDFACTORALPHA, D3DTA_CURRENT );\
			}


	/**************************************************************************
	*	类声明
	**************************************************************************/

	DLL_CLASS MDxUtil
	{
	public:
		// 矩形校验
		static void CheckOutUpdateRect( RECT& rcDest, int X, int Y, RECT& rcSrc );
		static void CheckOutStretchRect( RECT& rcDest, int width, int height, RECT& rcSrc );
	};
}

#endif // __MLT_DX_UTIL_H__









