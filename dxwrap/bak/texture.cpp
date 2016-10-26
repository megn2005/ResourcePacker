/******************************************************************************
*	filename:	texture.cpp
*	created:	8/1/2010
*	author:		Loser
*
*	purpose:	纹理
******************************************************************************/

#include <time.h>

#include <map>
using namespace std;

#include <d3d9.h>
#include <d3dx9.h>

#define MLT_DLL_EXPORT

#include "../../../core/include/core.h"

#include "../include/graph.h"
#include "../include/util.h"
#include "../include/direct3d.h"
#include "../include/texture.h"

namespace mlt
{
	//-------------------------------------------------------------------------
	//
	MTexture::MTexture( MDirect3D* pD3D ) : m_pD3D( pD3D )
	{
		m_pITexture = NULL;
		m_pISurface = NULL;
		memset( &m_surfaceDesc, 0, sizeof( m_surfaceDesc ));

		m_iWidth = m_iHeight = 0;
	}

	//-------------------------------------------------------------------------
	//
	MTexture::~MTexture()
	{
		Release();
	}

	//-------------------------------------------------------------------------
	//
	bool MTexture::Create( UINT uiWidth, UINT uiHeight, D3DPOOL pool, D3DFORMAT format )
	{
		// 使用方式
		DWORD dwUsage = 0;

		switch ( pool )
		{
		case D3DPOOL_SYSTEMMEM:	dwUsage |= D3DUSAGE_DYNAMIC; break;
		case D3DPOOL_DEFAULT:	dwUsage |= D3DUSAGE_RENDERTARGET; break;
		}

		// 创建
		PDIRECT3DDEVICE9 pIDevice = m_pD3D->GetIDevice();
		if ( pIDevice )
		{
			if ( SUCCEEDED( pIDevice->CreateTexture( uiWidth, uiHeight, 1, dwUsage, format, pool, &m_pITexture, NULL )) || 
				 SUCCEEDED( D3DXCreateTexture( pIDevice, uiWidth, uiHeight, 1, dwUsage, format, pool, &m_pITexture )))
			{
				m_pITexture->GetLevelDesc( 0, &m_surfaceDesc );

				m_iWidth = uiWidth;
				m_iHeight = uiHeight;

				return true;
			}
		}

		return false;
	}

	//-------------------------------------------------------------------------
	//
	bool MTexture::Create( LPCSTR pFilename, DWORD dwColorKey )
	{
		D3DXIMAGE_INFO imgInfo;

		if ( SUCCEEDED( D3DXGetImageInfoFromFile( pFilename, &imgInfo )))
		{
			PDIRECT3DDEVICE9 pIDevice = m_pD3D->GetIDevice();
			if ( pIDevice )
			{
				// SUCCEEDED( D3DXCreateTextureFromFile( PD3DDEVICE, filename, &m_pITexture )))
				if ( SUCCEEDED( D3DXCreateTextureFromFileEx( pIDevice, pFilename,
															imgInfo.Width, imgInfo.Height,
															imgInfo.MipLevels, 0,
															imgInfo.Format, D3DPOOL_MANAGED,
															D3DX_DEFAULT, D3DX_DEFAULT,	
															dwColorKey, NULL, NULL,
															&m_pITexture )))
				{
					m_pITexture->GetLevelDesc( 0, &m_surfaceDesc );

					m_iWidth = imgInfo.Width;
					m_iHeight = imgInfo.Height;

					return true;
				}
			}
		}

		return false;
	}

	//-------------------------------------------------------------------------
	//
	void MTexture::Release()
	{
		//
		// MRenderManager::GetInst().DelRenderNode( this );

		//
		m_iWidth = m_iHeight = 0;

		//
		SAFE_RELEASE( m_pITexture );
		SAFE_RELEASE( m_pISurface );
		memset( &m_surfaceDesc, 0, sizeof( m_surfaceDesc ));
	}

	//-------------------------------------------------------------------------
	// this pSrcRect can not be NULL
	bool MTexture::Load( const RECT* pDestRect, LPCVOID pData, UINT uiSrcPitch, const PRECT pSrcRect, DWORD dwColorKey )
	{
		PDIRECT3DSURFACE9 pISurface = GetISurface();
		if ( pISurface && pData )
		{
			return SUCCEEDED( D3DXLoadSurfaceFromMemory( pISurface, NULL, pDestRect, 
													pData, m_surfaceDesc.Format, uiSrcPitch, NULL, pSrcRect, 
													D3DX_DEFAULT, dwColorKey ));
		}

		return false;
	}

	//-------------------------------------------------------------------------
	//
	bool MTexture::Copy( const RECT* pDestRect, MTexture* pSrcTex, const PRECT pSrcRect, DWORD dwColorKey )
	{
		if ( pSrcTex )
		{
			PDIRECT3DDEVICE9 pIDevice = m_pD3D->GetIDevice();
			PDIRECT3DSURFACE9 pIDestSurface = GetISurface();
			PDIRECT3DSURFACE9 pISrcSurface = pSrcTex->GetISurface();

			if ( pIDevice && pIDestSurface && pISrcSurface )
			{
				if ( D3DPOOL_DEFAULT == m_surfaceDesc.Pool )
				{
					//
					if ( D3DPOOL_SYSTEMMEM == pSrcTex->m_surfaceDesc.Pool )
					{
						MTexture* pTexture = m_pD3D->Append( pSrcTex );
						if ( pTexture )
						{
							pSrcTex = pTexture;
							pISrcSurface = pTexture->GetISurface();
						}
					}

					//
					if ( D3DPOOL_DEFAULT == pSrcTex->m_surfaceDesc.Pool )
					{
						return SUCCEEDED( pIDevice->StretchRect( pISrcSurface, pSrcRect, 
																pIDestSurface, pDestRect, 
																D3DTEXF_NONE ));
					}
				}
				
				//
				return SUCCEEDED( D3DXLoadSurfaceFromSurface( pIDestSurface, NULL, pDestRect, 
															pISrcSurface, NULL, pSrcRect, 
															D3DX_DEFAULT, dwColorKey ));
			}
		}

		return false;
	}

	//-------------------------------------------------------------------------
	//
	bool MTexture::Fill( const RECT* pRect, DWORD dwColor )
	{
		PDIRECT3DDEVICE9 pIDevice = m_pD3D->GetIDevice();

		if ( m_pITexture && pIDevice )
		{
			// 填充
			if ( D3DPOOL_DEFAULT == m_surfaceDesc.Pool )
				return SUCCEEDED( pIDevice->ColorFill( GetISurface(), pRect, dwColor ));
			else
			{
				D3DLOCKED_RECT rcLocked;

				if ( SUCCEEDED( m_pITexture->LockRect( 0, &rcLocked, pRect, D3DLOCK_NOSYSLOCK | D3DLOCK_DISCARD )) )
				{
					int width = pRect ? ( pRect->right - pRect->left ) : m_iWidth;
					int height = pRect ? ( pRect->bottom - pRect->top ) : m_iHeight;

					switch ( m_surfaceDesc.Format )
					{
					case D3DFMT_A8R8G8B8:
					case D3DFMT_X8R8G8B8:
						for ( int i = 0; i < width; i++ )
							*(( PDWORD )(( PBYTE )rcLocked.pBits + i * 4 )) = dwColor;
						for ( int j = 1; j < height; j++ )
							memcpy(( PBYTE )rcLocked.pBits + j * rcLocked.Pitch, rcLocked.pBits, width * 4 );
						break;
					}

					return SUCCEEDED( m_pITexture->UnlockRect( 0 )) /* && Dirty( pRect );*/;	 // 解锁 && 脏标记
				}
			}
		}

		return false;
	}

	//-------------------------------------------------------------------------
	//
	DWORD MTexture::GetPixel( int X, int Y )
	{
		DWORD dwColor(0);
		D3DLOCKED_RECT rcLocked;

		if ( m_pITexture && SUCCEEDED( m_pITexture->LockRect( 0, &rcLocked, NULL, 
															D3DLOCK_NOSYSLOCK | D3DLOCK_DISCARD )))
		{
			switch ( m_surfaceDesc.Format )
			{
			case D3DFMT_A8R8G8B8:
			case D3DFMT_X8R8G8B8:
				dwColor = *(( PDWORD )(( PBYTE )rcLocked.pBits + X * 4 + Y * rcLocked.Pitch ));
				break;
			}
			m_pITexture->UnlockRect( 0 );
		}

		return dwColor;
	}

	//-------------------------------------------------------------------------
	//
	void MTexture::SetPixel( int X, int Y, DWORD dwColor )
	{
		D3DLOCKED_RECT rcLocked;

		if ( m_pITexture && SUCCEEDED( m_pITexture->LockRect( 0, &rcLocked, NULL, 
															D3DLOCK_NOSYSLOCK | D3DLOCK_DISCARD )))
		{
			switch ( m_surfaceDesc.Format )
			{
			case D3DFMT_A8R8G8B8:
			case D3DFMT_X8R8G8B8:
				*(( PDWORD )(( PBYTE )rcLocked.pBits + X * 4 + Y * rcLocked.Pitch )) = dwColor;
				break;
			}
			m_pITexture->UnlockRect( 0 );
		}
	}

	//-------------------------------------------------------------------------
	//
	PDIRECT3DSURFACE9 MTexture::GetISurface( UINT level )
	{
		static UINT uiSurfaceLv = -1;

		if ( level != uiSurfaceLv )
		{
			SAFE_RELEASE( m_pISurface );
		}

		if ( !m_pISurface && m_pITexture )
		{
			if ( SUCCEEDED( m_pITexture->GetSurfaceLevel( level, &m_pISurface )))
			{
				uiSurfaceLv = level;
			}
		}

		return m_pISurface;
	}

	//-------------------------------------------------------------------------
	//
}







