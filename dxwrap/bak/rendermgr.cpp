/******************************************************************************
*	filename:	rendermanager.cpp
*	created:	8/26/2010
*	author:		Loser
*
*	purpose:
******************************************************************************/

#include <time.h>

#include <map>
#include <deque>
#include <stack>
using namespace std;



#define MLT_DLL_EXPORT

#include "../../../core/include/core.h"
#include "../../../algorithm/include/list.h"
#include "../../../algorithm/include/memory.h"

#include "../include/graph.h"
#include "../include/util.h"
#include "../include/direct3d.h"
#include "../include/texture.h"
#include "../include/vertexbuffer.h"
#include "../include/rendermanager.h"

namespace mlt
{
	//-------------------------------------------------------------------------
	//
	MRenderManager::MRenderManager( MDirect3D* pD3D ) : m_pD3D( pD3D )
	{
		//
		m_pIRTSurface = NULL;

		//
		InitializeCriticalSection( &m_csRenderQueue );
		m_deqRenderQueue.clear();
	}

	//-------------------------------------------------------------------------
	//
	MRenderManager::~MRenderManager()
	{
		m_pIRTSurface = NULL;

		Clear();
		DeleteCriticalSection( &m_csRenderQueue );
	}

	//-------------------------------------------------------------------------
	//
	bool MRenderManager::Push( PRENDER_NODE pNode )
	{
		if ( pNode )
		{
			// 添加节点
			switch ( pNode->enType )
			{
			case ndOp: // 操作节点
				return addOperateNode( pNode );

			case ndInfo: // 信息节点
				switch ( pNode->rdInfo.primitiveType )
				{
				case D3DPT_TRIANGLESTRIP: return addQuadNode( pNode );
				case D3DPT_LINESTRIP: return addLineNode( pNode );
				}
				break;
			}
		}

		return false;
	}

	//-------------------------------------------------------------------------
	//
	void MRenderManager::Pop( PRENDER_NODE pNode )
	{
		if ( pNode )
		{
			EnterCriticalSection( &m_csRenderQueue );
			RENDER_QUEUE::iterator itEnd = m_deqRenderQueue.end();
			for ( RENDER_QUEUE::iterator it = m_deqRenderQueue.begin(); it != itEnd; it++ )
			{
				if ( !memcmp( &*it, pNode, sizeof( RENDER_NODE )))
				{
					m_deqRenderQueue.erase( it );
					break;
				}
			}
			LeaveCriticalSection( &m_csRenderQueue );
		}
	}

	//-------------------------------------------------------------------------
	// 
	void MRenderManager::Render( MVertexBuffer* pBuffer )
	{
		PDIRECT3DDEVICE9 pIDevice = m_pD3D->GetIDevice();
		if ( pIDevice && pBuffer )
		{
			EnterCriticalSection( &m_csRenderQueue );
			UINT nSize = m_deqRenderQueue.size();
			LeaveCriticalSection( &m_csRenderQueue );
			if ( 0 < nSize )
				pIDevice->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0 );

			for ( UINT i = 0; i < nSize; i++ )
			{
				EnterCriticalSection( &m_csRenderQueue );
				RENDER_NODE& nd = m_deqRenderQueue.front();
				LeaveCriticalSection( &m_csRenderQueue );
				switch ( nd.enType )
				{
				case ndOp: // 操作节点
					switch ( nd.rdOp.enType )
					{
					case opStage: renderStage( nd ); break;
					case opTest: renderTest( nd ); break;
					case opBlend: renderBlend( nd ); break;
					case opTarget: renderTarget( nd ); break;
					}
					break;

				case ndInfo: // 信息节点
					switch ( nd.rdInfo.primitiveType )
					{
					case D3DPT_TRIANGLESTRIP: renderQuad( nd ); break;
					case D3DPT_LINESTRIP: renderLine( nd ); break;
					}

					// 绘制
					pIDevice->DrawPrimitive( nd.rdInfo.primitiveType, 
											nd.rdInfo.nStartVertex, 
											nd.rdInfo.nPrimitiveCnt );
					break;
				}

				//
				pBuffer->Pop( &nd );
				EnterCriticalSection( &m_csRenderQueue );
				m_deqRenderQueue.pop_front();
				LeaveCriticalSection( &m_csRenderQueue );
			}
		}
	}

	//-------------------------------------------------------------------------
	//
	void MRenderManager::Clear()
	{
		EnterCriticalSection( &m_csRenderQueue );
		m_deqRenderQueue.clear();
		LeaveCriticalSection( &m_csRenderQueue );
	}

	//-------------------------------------------------------------------------
	//
	bool MRenderManager::addOperateNode( PRENDER_NODE pNode )
	{
		EnterCriticalSection( &m_csRenderQueue );
		m_deqRenderQueue.push_back( *pNode );
		LeaveCriticalSection( &m_csRenderQueue );
		return true;
	}

	//-------------------------------------------------------------------------
	//
	bool MRenderManager::addQuadNode( PRENDER_NODE pNode )
	{
		EnterCriticalSection( &m_csRenderQueue );
		m_deqRenderQueue.push_back( *pNode );
		LeaveCriticalSection( &m_csRenderQueue );
		return true;
	}

	//-------------------------------------------------------------------------
	//
	bool MRenderManager::addLineNode( PRENDER_NODE pNode )
	{
		//if ( !pNode->rdInfo.nPrimitiveCnt ) // 线段起始点
		//{
		//	pNode->rdInfo.nStartVertex = m_uiFillVxCnt;
		//	m_deqRenderQueue.push_back( *pNode );
		//}
		//else
		//	m_deqRenderQueue.back().rdInfo.nPrimitiveCnt++;

		return true;
	}

	//-------------------------------------------------------------------------
	//
	void MRenderManager::renderStage( RENDER_NODE& node )
	{
		if ( node.rdOp.stage.mode )
		{
			// 纹理阶段状态入栈
			pushTextureStageState();

			// 纹理阶段状态切换
			//switch ( pNode->rdOp.stage.mode )
			//{
			//
			//}
		}
		else 
		{
			// 纹理阶段状态出栈
			popTextureStageState();
		}
	}

	//-------------------------------------------------------------------------
	//
	void MRenderManager::renderTest( RENDER_NODE& node )
	{
		PDIRECT3DDEVICE9 pIDevice = m_pD3D->GetIDevice();

		if ( node.rdOp.test.mode )
		{
			// 测试状态入栈
			pushTestState();

			// 
			switch ( node.rdOp.test.mode )
			{
			case tmAlpha: SetRenderAlphaTest( pIDevice, node.rdOp.test.ref, node.rdOp.test.func ); break;
			}
		}
		else 
		{
			// 测试状态出栈
			popTestState();
		}
	}

	//-------------------------------------------------------------------------
	//
	void MRenderManager::renderBlend( RENDER_NODE& node )
	{
		PDIRECT3DDEVICE9 pIDevice = m_pD3D->GetIDevice();

		if ( node.rdOp.blend.mode )
		{
			// 渲染状态入栈
			pushRenderState();

			// 渲染状态切换
			switch ( node.rdOp.blend.mode )
			{
			case tpAlpha: SetRenderAlphaBlend( pIDevice, D3DBLEND_SRCALPHA, D3DBLENDOP_ADD, D3DBLEND_INVSRCALPHA ); break;
			case tpGlass: SetRenderAlphaBlend( pIDevice, D3DBLEND_SRCCOLOR, D3DBLENDOP_ADD, D3DBLEND_INVSRCCOLOR ); break;
			case tpFactor: SetTextureTransprt( pIDevice, node.rdOp.blend.factor ); break;
			case ltColorAdd: SetRenderAlphaBlend( pIDevice, D3DBLEND_ONE, D3DBLENDOP_ADD, D3DBLEND_DESTCOLOR ); break;
			case ltAddLight: SetRenderAlphaBlend( pIDevice, D3DBLEND_ONE, D3DBLENDOP_ADD, D3DBLEND_ONE ); break;
			case ltColorMul: SetRenderAlphaBlend( pIDevice, D3DBLEND_DESTCOLOR, D3DBLENDOP_ADD, D3DBLEND_ZERO ); break;
			case ltMulLight: SetRenderAlphaBlend( pIDevice, D3DBLEND_DESTCOLOR, D3DBLENDOP_ADD, D3DBLEND_SRCCOLOR ); break;
			case blColorUp: SetTextureColorUp( pIDevice, node.rdOp.blend.factor ); break;

			///////////////////////////////////////////////////////////////////
			// 盛大方式
			case sdFxNone: pIDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE ); break;
			case sdFxAdd: SetRenderAlphaBlend( pIDevice, D3DBLEND_SRCALPHA, D3DBLENDOP_ADD, D3DBLEND_DESTALPHA ); break;
			case sdFxAddX: SetRenderAlphaBlend( pIDevice, D3DBLEND_ONE, D3DBLENDOP_ADD, D3DBLEND_ONE );	break;
			case sdFxSub: SetRenderAlphaBlend( pIDevice, D3DBLEND_ONE,  D3DBLENDOP_REVSUBTRACT, D3DBLEND_ONE ); break;
			case sdFxLight: SetRenderAlphaBlend( pIDevice, D3DBLEND_ONE, D3DBLENDOP_ADD, D3DBLEND_SRCCOLOR ); break;
			case sdFxBright: SetRenderAlphaBlend( pIDevice, D3DBLEND_DESTALPHA, D3DBLENDOP_ADD, D3DBLEND_SRCCOLOR ); break;
			case sdFxBlend: SetRenderAlphaBlend( pIDevice, D3DBLEND_SRCALPHA, D3DBLENDOP_ADD, D3DBLEND_INVSRCALPHA ); break;
			case sdFxBlendNA: SetRenderAlphaBlend( pIDevice, D3DBLEND_SRCCOLOR, D3DBLENDOP_ADD, D3DBLEND_INVSRCCOLOR ); break;
			case sdFxBlend1X: SetRenderAlphaBlend( pIDevice, D3DBLEND_SRCALPHA, D3DBLENDOP_ADD, D3DBLEND_INVSRCCOLOR ); break;
			case sdFxBlend2X: SetRenderAlphaBlend( pIDevice, D3DBLEND_ONE, D3DBLENDOP_ADD, D3DBLEND_INVSRCCOLOR ); break;
			case sdFxMultiply: SetRenderAlphaBlend( pIDevice, D3DBLEND_DESTCOLOR, D3DBLENDOP_ADD, D3DBLEND_ZERO ); break;
			case sdFxMultiplyAlpha: SetRenderAlphaBlend( pIDevice, D3DBLEND_DESTALPHA, D3DBLENDOP_ADD, D3DBLEND_ZERO ); break;
			case sdFxMax: SetRenderAlphaBlend( pIDevice, D3DBLEND_SRCCOLOR, D3DBLENDOP_MAX, D3DBLEND_DESTCOLOR ); break;
			case sdFxMin: SetRenderAlphaBlend( pIDevice, D3DBLEND_SRCCOLOR, D3DBLENDOP_MIN, D3DBLEND_DESTCOLOR ); break;
			case sdFxSrcColorAdd: SetRenderAlphaBlend( pIDevice, D3DBLEND_SRCCOLOR, D3DBLENDOP_ADD, D3DBLEND_ONE ); break;
			case sdFxInvert: SetRenderAlphaBlend( pIDevice, D3DBLEND_DESTCOLOR, D3DBLENDOP_ADD, D3DBLEND_ONE ); break;
			case sdFxSrcBright: SetRenderAlphaBlend( pIDevice, D3DBLEND_SRCCOLOR, D3DBLENDOP_ADD, D3DBLEND_SRCCOLOR ); break;
			case sdFxDestBright: SetRenderAlphaBlend( pIDevice, D3DBLEND_DESTCOLOR, D3DBLENDOP_ADD, D3DBLEND_DESTCOLOR ); break;
			case sdFxAddSmooth: SetRenderAlphaBlend( pIDevice, D3DBLEND_ONE, D3DBLENDOP_ADD, D3DBLEND_INVSRCCOLOR ); break;

			///////////////////////////////////////////////////////////////////
			//
			}
		}
		else 
		{
			// 渲染状态出栈
			popRenderState();
		}
	}

	//-------------------------------------------------------------------------
	//
	void MRenderManager::renderTarget( RENDER_NODE& node )
	{
		PDIRECT3DDEVICE9 pIDevice = m_pD3D->GetIDevice();

		if ( node.rdOp.target.pTexture )
		{
			// 渲染目标入栈
			pushRenderTarget();

			// 设置渲染目标
			if ( SUCCEEDED( pIDevice->SetRenderTarget( 0, node.rdOp.target.pTexture->GetISurface() )))
			{
				m_pIRTSurface = node.rdOp.target.pTexture->GetISurface();

				// 裁剪矩形
				PRECT pRcClip = &( node.rdOp.target.clipRect );

				if ( pRcClip->left < pRcClip->right && pRcClip->top < pRcClip->bottom )
				{
					pIDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE );
					pIDevice->SetScissorRect( pRcClip );
				}
				else
					pIDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );
			}
		}
		else 
		{
			// 渲染目标出栈
			popRenderTarget();
		}
	}

	//-------------------------------------------------------------------------
	//
	void MRenderManager::renderQuad( RENDER_NODE& node )
	{
		PDIRECT3DDEVICE9 pIDevice = m_pD3D->GetIDevice();
		
		// 纹理
		if ( node.rdInfo.pTexture )
		{
			if ( D3DPOOL_SYSTEMMEM == node.rdInfo.pTexture->GetDesc().Pool )
			{
				MTexture* pTexture = m_pD3D->Append( node.rdInfo.pTexture );
				if ( pTexture )
					pIDevice->SetTexture( 0, pTexture->GetITexture() );
			}
			else
				pIDevice->SetTexture( 0, node.rdInfo.pTexture->GetITexture() ); 
		}
		else
			pIDevice->SetTexture( 0, NULL );	
	}

	//-------------------------------------------------------------------------
	//
	void MRenderManager::renderLine( RENDER_NODE& node )
	{
		PDIRECT3DDEVICE9 pIDevice = m_pD3D->GetIDevice();

		pIDevice->SetTexture( 0, NULL );
		//pIDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	}

	//-------------------------------------------------------------------------
	//
	void MRenderManager::pushTextureStageState()
	{
		TSS_NODE node;
		PDIRECT3DDEVICE9 pIDevice = m_pD3D->GetIDevice();

		for ( int i = 0; i < 3; i++ )
		{
			node.szTSS[i][0][0].first = D3DTSS_COLOROP;
			pIDevice->GetTextureStageState( i, node.szTSS[i][0][0].first, &node.szTSS[i][0][0].second );

			if ( D3DTOP_DISABLE != node.szTSS[i][0][0].second )
			{
				node.szTSS[i][0][1].first = D3DTSS_COLORARG1;
				pIDevice->GetTextureStageState( i, node.szTSS[i][0][1].first, &node.szTSS[i][0][1].second );

				node.szTSS[i][0][2].first = D3DTSS_COLORARG2;
				pIDevice->GetTextureStageState( i, node.szTSS[i][0][2].first, &node.szTSS[i][0][2].second );
			}
			
			node.szTSS[i][1][0].first = D3DTSS_ALPHAOP;
			pIDevice->GetTextureStageState( i, node.szTSS[i][1][0].first, &node.szTSS[i][1][0].second );

			if ( D3DTOP_DISABLE != node.szTSS[i][1][0].second )
			{
				node.szTSS[i][1][1].first = D3DTSS_ALPHAARG1;
				pIDevice->GetTextureStageState( i, node.szTSS[i][1][1].first, &node.szTSS[i][1][1].second );

				node.szTSS[i][1][2].first = D3DTSS_ALPHAARG2;
				pIDevice->GetTextureStageState( i, node.szTSS[i][1][2].first, &node.szTSS[i][1][2].second );
			}
		}
		
		m_stkTextureStageState.push( node );
	}

	//-------------------------------------------------------------------------
	//
	void MRenderManager::pushTestState()
	{
		TS_NODE node;
		PDIRECT3DDEVICE9 pIDevice = m_pD3D->GetIDevice();

		node.szTS[0].first = D3DRS_ALPHATESTENABLE;
		pIDevice->GetRenderState( node.szTS[0].first, &node.szTS[0].second );

		node.szTS[1].first = D3DRS_ALPHAREF;
		pIDevice->GetRenderState( node.szTS[1].first, &node.szTS[1].second );

		node.szTS[2].first = D3DRS_ALPHAFUNC;
		pIDevice->GetRenderState( node.szTS[2].first, &node.szTS[2].second );

		m_stkTestState.push( node );
	}

	//-------------------------------------------------------------------------
	//
	void MRenderManager::pushRenderState()
	{
		RS_NODE node;
		PDIRECT3DDEVICE9 pIDevice = m_pD3D->GetIDevice();
		
		node.szRS[0].first = D3DRS_ALPHABLENDENABLE;
		pIDevice->GetRenderState( node.szRS[0].first, &node.szRS[0].second );

		node.szRS[1].first = D3DRS_SRCBLEND;
		pIDevice->GetRenderState( node.szRS[1].first, &node.szRS[1].second );

		node.szRS[2].first = D3DRS_DESTBLEND;
		pIDevice->GetRenderState( node.szRS[2].first, &node.szRS[2].second );

		node.szRS[3].first = D3DRS_BLENDOP;
		pIDevice->GetRenderState( node.szRS[3].first, &node.szRS[3].second );

		node.szRS[4].first = D3DRS_SEPARATEALPHABLENDENABLE;
		pIDevice->GetRenderState( node.szRS[4].first, &node.szRS[4].second );

		node.szRS[5].first = D3DRS_SRCBLENDALPHA;
		pIDevice->GetRenderState( node.szRS[5].first, &node.szRS[5].second );

		node.szRS[6].first = D3DRS_DESTBLENDALPHA;
		pIDevice->GetRenderState( node.szRS[6].first, &node.szRS[6].second );

		node.szRS[7].first = D3DRS_BLENDOPALPHA;
		pIDevice->GetRenderState( node.szRS[7].first, &node.szRS[7].second );

		node.szRS[8].first = D3DRS_TEXTUREFACTOR;
		pIDevice->GetRenderState( node.szRS[8].first, &node.szRS[8].second );

		node.szRS[9].first = D3DRS_BLENDFACTOR;
		pIDevice->GetRenderState( node.szRS[9].first, &node.szRS[9].second );

		m_stkRenderState.push( node );
	}

	//-------------------------------------------------------------------------
	//
	void MRenderManager::pushRenderTarget()
	{
		PDIRECT3DSURFACE9 pISurface = NULL;
		PDIRECT3DDEVICE9 pIDevice = m_pD3D->GetIDevice();

		if ( SUCCEEDED( pIDevice->GetRenderTarget( 0, &pISurface )))
		{
			m_stkRenderTarget.push( pISurface );
		}		
	}

	//-------------------------------------------------------------------------
	//
	void MRenderManager::popTextureStageState()
	{
		PDIRECT3DDEVICE9 pIDevice = m_pD3D->GetIDevice();

		if ( !m_stkTextureStageState.empty() )
		{
			PTSS_NODE pNode = &m_stkTextureStageState.top();

			for ( int i = 0; i < 3; i++ )
			{
				for ( int j = 0; j < 2; j++ )
				{
					pIDevice->SetTextureStageState( i, pNode->szTSS[i][j][0].first, pNode->szTSS[i][j][0].second );

					if ( D3DTOP_DISABLE != pNode->szTSS[i][j][0].second )
					{
						for ( int k = 1; k < 3; k++ )
							pIDevice->SetTextureStageState( i, pNode->szTSS[i][j][k].first, pNode->szTSS[i][j][k].second );
					}
				}
			}

			m_stkTextureStageState.pop();
		}
	}

	//-------------------------------------------------------------------------
	//
	void MRenderManager::popTestState() 
	{
		PDIRECT3DDEVICE9 pIDevice = m_pD3D->GetIDevice();

		if ( !m_stkTestState.empty() )
		{
			PTS_NODE pNode = &m_stkTestState.top();

			for ( int i = 0; i < 3; i++ )
			{
				pIDevice->SetRenderState( pNode->szTS[i].first, pNode->szTS[i].second );
			}

			m_stkTestState.pop();
		}
	}

	//-------------------------------------------------------------------------
	//
	void MRenderManager::popRenderState()
	{
		PDIRECT3DDEVICE9 pIDevice = m_pD3D->GetIDevice();

		if ( !m_stkRenderState.empty() )
		{
			PRS_NODE pNode = &m_stkRenderState.top();

			for ( int i = 0; i < 10; i++ )
			{
				pIDevice->SetRenderState( pNode->szRS[i].first, pNode->szRS[i].second );
			}

			m_stkRenderState.pop();
		}
	}

	//-------------------------------------------------------------------------
	//
	void MRenderManager::popRenderTarget()
	{
		PDIRECT3DDEVICE9 pIDevice = m_pD3D->GetIDevice();

		if ( !m_stkRenderTarget.empty() )
		{
			m_pIRTSurface = m_stkRenderTarget.top();
			pIDevice->SetRenderTarget( 0, m_pIRTSurface );

			m_pIRTSurface->Release();
			m_stkRenderTarget.pop();
		}

		if ( m_stkRenderTarget.empty() )
			m_pIRTSurface = NULL;
	}

	//-------------------------------------------------------------------------
	//
	void MRenderManager::restoreState()
	{
		PDIRECT3DDEVICE9 pIDevice = m_pD3D->GetIDevice();

		// 默认渲染状态
		//pIDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
		pIDevice->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, FALSE );

		// 默认混合状态
		//pIDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		//pIDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
		SetTextureColorStage( pIDevice, 0, D3DTA_TEXTURE, D3DTOP_MODULATE, D3DTA_CURRENT );
		SetTextureAlphaStage( pIDevice, 0, D3DTA_TEXTURE, D3DTOP_SELECTARG1, D3DTA_CURRENT );

		for ( int i = 1; i < 8; i++ )
		{
			pIDevice->SetTextureStageState( i, D3DTSS_COLOROP, D3DTOP_DISABLE );
			pIDevice->SetTextureStageState( i, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
		}

		// 混合因子
		pIDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFFFFFFFF );
		pIDevice->SetRenderState( D3DRS_BLENDFACTOR, 0xFFFFFFFF );
	}

	//-------------------------------------------------------------------------
	//
}



