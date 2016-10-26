/******************************************************************************
*	filename:	vertexbuf.h
*	created:	8/17/2010
*	author:		Loser
*
*	purpose:	���㻺��
******************************************************************************/

#ifndef __MLT_DX_VERTEX_BUF_H__
#define __MLT_DX_VERTEX_BUF_H__

namespace mlt
{
	/**************************************************************************
	*
	**************************************************************************/

	//
	#define	MAX_VERTEX_BUF_SIZE		10*1024							// 10KB
	#define D3DFVF_PLANE			D3DFVF_XYZRHW | D3DFVF_TEX1		// �����ڽṹ�����棬��Сÿ�������С
	#define D3DFVF_SOLID			D3DFVF_XYZ | D3DFVF_TEX1

	// ƽ�涥���ʽ
	struct QuadVertex
	{
		//
		QuadVertex( float _x = 0, float _y = 0, float _z = 1, float _rhw = 1, 
					float _u = 0, float _v = 0 ) : 
					x(_x), y(_y), z(_z), rhw(_rhw), 
					u(_u), v(_v) {}

		//
		float x, y, z, rhw;	// ��������
		float u, v;			// ��������
	};


	/**************************************************************************
	*
	**************************************************************************/

	class MTexture;

	// ��Ⱦ��������
	typedef enum
	{
		// opNone	= 0,
		opStage		= 1,	// texture stage
		opTest		= 2,	// alpha test
		opBlend		= 3,	// alpha blending
		opTarget	= 4,	// render target

	} RENDER_OP_TYPE_E;

	// ��Ⱦ�����ڵ�
	typedef struct _render_op_node
	{
		// ��������
		RENDER_OP_TYPE_E enType;

		// ��������
		union
		{
			// texture stage
			struct _stage
			{
				BYTE		mode;	
				DWORD		factor;
				MTexture*	pTexture;

			} stage;

			// alpha test
			struct _test
			{
				BYTE	mode;	// ģʽ
				BYTE	ref;	// �ο�ֵ
				BYTE	func;	// �ȽϺ���

			} test;

			// alpha blend
			struct _blend
			{
				BYTE	mode;	// ���ģʽ
				DWORD	factor;	// �������

			} blend;

			// render target
			struct _target
			{
				MTexture*	pTexture;	// ��ȾĿ��
				RECT		clipRect;	// �ü�����

			} target;
		};

	} RENDER_OP_NODE, *PRENDER_OP_NODE;

	// ��Ⱦ��Ϣ�ڵ�
	typedef struct _render_info_node
	{
		// ������Ϣ
		D3DPRIMITIVETYPE	primitiveType;		// ��������
		UINT				nStartVertex;		// ����ƫ��
		UINT				nPrimitiveCnt;		// ͼԪ����

		//
		RECT		destRect;	// Ŀ�����
		float		z;			// Z������
		D3DCOLOR	color;		// ������ɫ
		
		// ������Ϣ
		MTexture*	pTexture;	// Դ����
		RECT		srcRect;	// Դ����
		
	} RENDER_INFO_NODE, *PRENDER_INFO_NODE;


	// �ڵ�����
	typedef enum
	{
		//ndNone = 0,
		ndOp	= 1,	// �����ڵ�
		ndInfo	= 2,	// ��Ϣ�ڵ�

	} RENDER_NODE_TYPE_E;

	// ��Ⱦ�ڵ�
	typedef struct _render_node
	{
		// �ڵ�����
		RENDER_NODE_TYPE_E enType;

		// �ڵ���Ϣ
		union
		{
			RENDER_OP_NODE		rdOp;
			RENDER_INFO_NODE	rdInfo;
		};

	} RENDER_NODE, *PRENDER_NODE;


	/**************************************************************************
	*
	**************************************************************************/

	class MVertexBuf
	{
	public:
		MVertexBuf(MDirect3D* pD3D = MDirect3D::GetInst());
		virtual ~MVertexBuf();

		bool Initialize(UINT nLength, DWORD dwFvf, D3DPOOL pool = D3DPOOL_DEFAULT);
		void Finalize();

		// �������񻺴�
		bool Push(PRENDER_NODE pNode);
		void Pop(PRENDER_NODE pNode);

		// ͬ�����㻺��
		void Flush();

		// ��վ��񻺴�
		void Clear();

	public:
		inline PDIRECT3DVERTEXBUFFER9 GetIBuffer() { return m_pIVertexBuffer; }

	private:
		bool addQuadNode( PRENDER_NODE pNode );
		bool addLineNode( PRENDER_NODE pNode );
		
	private:
		MDirect3D*	m_pD3D;

		// �������
		PDIRECT3DVERTEXBUFFER9	m_pIVertexBuffer;
		D3DVERTEXBUFFER_DESC	m_VertexBufferDesc;

		// ���㾵�񻺴�
		CRITICAL_SECTION	m_csBuffer;
		MCycleMem*			m_pBuffer;
	};
}

#endif // __MLT_DX_VERTEX_BUFFER_H__




