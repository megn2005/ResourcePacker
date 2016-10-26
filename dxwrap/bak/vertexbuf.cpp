/******************************************************************************
*	filename:	vertexbuffer.cpp
*	created:	8/17/2010
*	author:		Loser
*
*	purpose:	顶点缓存
******************************************************************************/

#include <time.h>

#include <map>
using namespace std;

#include <d3d9.h>
#include <d3dx9.h>

#define MLT_DLL_EXPORT

#include "../include/graph.h"
#include "../include/util.h"
#include "../include/direct3d.h"
#include "../include/texture.h"
#include "../include/vertexbuffer.h"

namespace mlt
{
	//-------------------------------------------------------------------------
	//
	MVertexBuffer::MVertexBuffer( MDirect3D* pD3D ) : m_pD3D( pD3D )
	{
		//
		m_pIVertexBuffer = NULL;
		memset( &m_VertexBufferDesc, 0, sizeof( m_VertexBufferDesc ));

		//
		InitializeCriticalSection( &m_csBuffer );
		m_pBuffer = NULL;
	}

	//-------------------------------------------------------------------------
	//
	MVertexBuffer::~MVertexBuffer()
	{
		Finalize();
		DeleteCriticalSection( &m_csBuffer );
	}

	//-------------------------------------------------------------------------
	//
	bool MVertexBuffer::Initialize( UINT nLength, DWORD dwFvf, D3DPOOL pool )
	{
		// 使用方式
		DWORD dwUsage = 0;

		switch ( pool )
		{
		//case D3DPOOL_MANAGED:
		case D3DPOOL_DEFAULT:	dwUsage |= D3DUSAGE_WRITEONLY; break;
		case D3DPOOL_SYSTEMMEM:	dwUsage |= D3DUSAGE_DYNAMIC; break;
		}

		// 创建
		PDIRECT3DDEVICE9 pIDevice = m_pD3D->GetIDevice();
		if ( pIDevice )
		{
			if ( SUCCEEDED( pIDevice->CreateVertexBuffer( nLength, dwUsage, dwFvf, pool, &m_pIVertexBuffer, NULL )))
			{
				m_pIVertexBuffer->GetDesc( &m_VertexBufferDesc );

				// 初始化缓存
				PVOID pBuffer = NULL;

				if ( SUCCEEDED( m_pIVertexBuffer->Lock( 0, 0, &pBuffer, D3DLOCK_NOSYSLOCK | D3DLOCK_DISCARD )) )	 
				{
					memset( pBuffer, 0, m_VertexBufferDesc.Size );
					m_pIVertexBuffer->Unlock();
				}

				// 初始化缓存镜像
				m_pBuffer = new MCycleMem( nLength );
				return NULL != m_pBuffer;
			}
		}

		return false;
	}

	//-------------------------------------------------------------------------
	//
	void MVertexBuffer::Finalize()
	{
		//
		SAFE_RELEASE( m_pIVertexBuffer );
		memset( &m_VertexBufferDesc, 0, sizeof( m_VertexBufferDesc ));

		//
		DELETE_PTR( m_pBuffer );
	}

	//-------------------------------------------------------------------------
	//
	bool MVertexBuffer::Push( PRENDER_NODE pNode )
	{
		if ( pNode && m_pBuffer )
		{
			// 添加节点
			switch ( pNode->enType )
			{
			case ndOp:		// 操作节点
				return true;

			case ndInfo:	// 信息节点
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
	void MVertexBuffer::Pop( PRENDER_NODE pNode )
	{
		if ( pNode && m_pBuffer )
		{
			// 释放节点
			switch ( pNode->enType )
			{
			case ndOp:		// 操作节点
				break;

			case ndInfo:	// 信息节点
				switch ( pNode->rdInfo.primitiveType )
				{
				case D3DPT_TRIANGLESTRIP:
				case D3DPT_LINESTRIP:
					EnterCriticalSection( &m_csBuffer );
					m_pBuffer->Free( (QuadVertex*)m_pBuffer->GetAddress() + pNode->rdInfo.nStartVertex );
					LeaveCriticalSection( &m_csBuffer );
					break;
				}
				break;
			}
		}
	}

	//-------------------------------------------------------------------------
	//
	void MVertexBuffer::Flush()
	{
		if ( m_pIVertexBuffer && m_pBuffer )
		{
			PVOID pBuffer = NULL;
			if ( SUCCEEDED( m_pIVertexBuffer->Lock( 0,0, &pBuffer, D3DLOCK_NOSYSLOCK /*| D3DLOCK_DISCARD*/ )) ) // 加上 D3DLOCK_DISCARD 会报错
			{
				memcpy( pBuffer, m_pBuffer->GetAddress(), m_pBuffer->GetSize() );
				m_pIVertexBuffer->Unlock();		
			}
		}
	}

	//-------------------------------------------------------------------------
	//
	void MVertexBuffer::Clear()
	{
		if ( m_pBuffer )
		{
			EnterCriticalSection( &m_csBuffer );
			m_pBuffer->Clear();
			LeaveCriticalSection( &m_csBuffer );
		}
	}

	//-------------------------------------------------------------------------
	//
	bool MVertexBuffer::addQuadNode( PRENDER_NODE pNode )
	{
		///////////////////////////////////////////////////////////////////////
		// 顶点
		EnterCriticalSection( &m_csBuffer );
		QuadVertex* pQV = ( QuadVertex* )m_pBuffer->Malloc( 4 * sizeof( QuadVertex ));
		LeaveCriticalSection( &m_csBuffer );
		if ( pQV )
		{
			// 顶点坐标
			pQV[1].x = pQV[0].x = (float)pNode->rdInfo.destRect.left - 0.5f;
			pQV[3].x = pQV[2].x = (float)pNode->rdInfo.destRect.right - 0.5f;

			pQV[1].y = pQV[3].y = (float)pNode->rdInfo.destRect.top - 0.5f;
			pQV[0].y = pQV[2].y = (float)pNode->rdInfo.destRect.bottom - 0.5f;

			pQV[0].z = pQV[1].z = pQV[2].z = pQV[3].z = pNode->rdInfo.z;
			pQV[0].rhw = pQV[1].rhw = pQV[2].rhw = pQV[3].rhw = 1;

			// 顶点偏移
			pNode->rdInfo.nStartVertex = (( PBYTE )pQV - ( PBYTE )m_pBuffer->GetAddress() ) / sizeof( QuadVertex );
			pNode->rdInfo.nPrimitiveCnt = 2;

			///////////////////////////////////////////////////////////////////////
			// 纹理
			if ( pNode->rdInfo.pTexture )
			{
				// 纹理坐标
				if ( 0 < pNode->rdInfo.pTexture->GetWidth() && 0 < pNode->rdInfo.pTexture->GetHeight() )
				{
					pQV[1].u = pQV[0].u = (float)pNode->rdInfo.srcRect.left/(float)pNode->rdInfo.pTexture->GetDesc().Width;
					pQV[3].u = pQV[2].u = (float)pNode->rdInfo.srcRect.right/(float)pNode->rdInfo.pTexture->GetDesc().Width;

					pQV[1].v = pQV[3].v = (float)pNode->rdInfo.srcRect.top/(float)pNode->rdInfo.pTexture->GetDesc().Height;
					pQV[0].v = pQV[2].v = (float)pNode->rdInfo.srcRect.bottom/(float)pNode->rdInfo.pTexture->GetDesc().Height;
				}
			}

			return true;
		}
		
		return false;
	}

	//-------------------------------------------------------------------------
	//
	bool MVertexBuffer::addLineNode( PRENDER_NODE pNode )
	{
		///////////////////////////////////////////////////////////////////////
		// 顶点
		EnterCriticalSection( &m_csBuffer );
		QuadVertex* pQV = ( QuadVertex* )m_pBuffer->Malloc( sizeof( QuadVertex ));
		LeaveCriticalSection( &m_csBuffer );
		if ( pQV )
		{
			// 顶点坐标
			pQV->x = (float)pNode->rdInfo.destRect.left - 0.5f;
			pQV->y = (float)pNode->rdInfo.destRect.top - 0.5f;
			pQV->z = pNode->rdInfo.z;
			pQV->rhw = 1;

			// 顶点偏移
			if ( !pNode->rdInfo.nPrimitiveCnt ) // 线段起始点
				pNode->rdInfo.nStartVertex = (( PBYTE )pQV - ( PBYTE )m_pBuffer->GetAddress() ) / sizeof( QuadVertex );
			//else
			//	m_deqRenderQueue.back().rdInfo.nPrimitiveCnt++;

			return true;
		}

		return false;
	}

	//-------------------------------------------------------------------------
	//
}

