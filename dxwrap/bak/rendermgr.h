/******************************************************************************
*	filename:	rendermanager.h
*	created:	8/26/2010
*	author:		Loser
*
*	purpose:	
******************************************************************************/

#ifndef __MLT_DX_RENDER_MANAGER_H__
#define __MLT_DX_RENDER_MANAGER_H__

namespace mlt
{
	//=========================================================================
	//	������
	//=========================================================================

	DLL_CLASS MRenderManager
	{
	public:
		MRenderManager( MDirect3D* pD3D = MDirect3D::GetInst() );
		virtual ~MRenderManager();

		// ������Ⱦ����
		bool Push( PRENDER_NODE pNode );
		void Pop( PRENDER_NODE pNode );
		
		// ��Ⱦ����
		void Render( MVertexBuffer* pBuffer );

		// �����Ⱦ����
		void Clear();

	public: 
		inline PDIRECT3DSURFACE9 GetRenderTarget() { return m_pIRTSurface; }

	protected:
		// ��Ⱦ����
		bool addOperateNode( PRENDER_NODE pNode );
		bool addQuadNode( PRENDER_NODE pNode );
		bool addLineNode( PRENDER_NODE pNode );

		// ��Ⱦ�ڵ�
		void renderStage( RENDER_NODE& node );
		void renderTest( RENDER_NODE& node );
		void renderBlend( RENDER_NODE& node );
		void renderTarget( RENDER_NODE& node );

		void renderQuad( RENDER_NODE& node );
		void renderLine( RENDER_NODE& node );

		// ��Ⱦ״̬��ջ
		void pushTextureStageState();
		void pushTestState();
		void pushRenderState();
		void pushRenderTarget();

		void popTextureStageState();
		void popTestState();
		void popRenderState();
		void popRenderTarget();

		void restoreState();

	private:
		typedef deque< RENDER_NODE >	RENDER_QUEUE;

		typedef struct { pair< D3DTEXTURESTAGESTATETYPE, DWORD > szTSS[3][2][3]; }	TSS_NODE, *PTSS_NODE;
		typedef struct { pair< D3DRENDERSTATETYPE, DWORD > szTS[3]; }				TS_NODE, *PTS_NODE;
		typedef struct { pair< D3DRENDERSTATETYPE, DWORD > szRS[10]; }				RS_NODE, *PRS_NODE;
		typedef stack< TSS_NODE >			TSS_STACK;
		typedef stack< TS_NODE >			TS_STACK;
		typedef stack< RS_NODE >			RS_STACK;
		typedef stack< PDIRECT3DSURFACE9 >	RT_STACK;
		
		// D3D����
		MDirect3D*	m_pD3D;

		// ��ǰ��ȾĿ��
		PDIRECT3DSURFACE9	m_pIRTSurface;

		// ��Ⱦ����
		CRITICAL_SECTION	m_csRenderQueue;
		RENDER_QUEUE		m_deqRenderQueue;

		// ״̬��ջ
		TSS_STACK	m_stkTextureStageState;
		TS_STACK	m_stkTestState;
		RS_STACK	m_stkRenderState;
		RT_STACK	m_stkRenderTarget;

	};
}

#endif // __MLT_DX_RENDER_MANAGER_H__














