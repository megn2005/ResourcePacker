/******************************************************************************
*	filename:	vertexbuf.h
*	created:	8/17/2010
*	author:		Loser
*
*	purpose:	顶点缓存
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
	#define D3DFVF_PLANE			D3DFVF_XYZRHW | D3DFVF_TEX1		// 定义在结构体外面，减小每个顶点大小
	#define D3DFVF_SOLID			D3DFVF_XYZ | D3DFVF_TEX1

	// 平面顶点格式
	struct QuadVertex
	{
		//
		QuadVertex( float _x = 0, float _y = 0, float _z = 1, float _rhw = 1, 
					float _u = 0, float _v = 0 ) : 
					x(_x), y(_y), z(_z), rhw(_rhw), 
					u(_u), v(_v) {}

		//
		float x, y, z, rhw;	// 顶点坐标
		float u, v;			// 纹理坐标
	};


	/**************************************************************************
	*
	**************************************************************************/

	class MTexture;

	// 渲染操作类型
	typedef enum
	{
		// opNone	= 0,
		opStage		= 1,	// texture stage
		opTest		= 2,	// alpha test
		opBlend		= 3,	// alpha blending
		opTarget	= 4,	// render target

	} RENDER_OP_TYPE_E;

	// 渲染操作节点
	typedef struct _render_op_node
	{
		// 操作类型
		RENDER_OP_TYPE_E enType;

		// 操作参数
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
				BYTE	mode;	// 模式
				BYTE	ref;	// 参考值
				BYTE	func;	// 比较函数

			} test;

			// alpha blend
			struct _blend
			{
				BYTE	mode;	// 混合模式
				DWORD	factor;	// 混合因子

			} blend;

			// render target
			struct _target
			{
				MTexture*	pTexture;	// 渲染目标
				RECT		clipRect;	// 裁剪矩形

			} target;
		};

	} RENDER_OP_NODE, *PRENDER_OP_NODE;

	// 渲染信息节点
	typedef struct _render_info_node
	{
		// 顶点信息
		D3DPRIMITIVETYPE	primitiveType;		// 顶点类型
		UINT				nStartVertex;		// 顶点偏移
		UINT				nPrimitiveCnt;		// 图元数量

		//
		RECT		destRect;	// 目标矩形
		float		z;			// Z轴坐标
		D3DCOLOR	color;		// 顶点颜色
		
		// 纹理信息
		MTexture*	pTexture;	// 源纹理
		RECT		srcRect;	// 源矩形
		
	} RENDER_INFO_NODE, *PRENDER_INFO_NODE;


	// 节点类型
	typedef enum
	{
		//ndNone = 0,
		ndOp	= 1,	// 操作节点
		ndInfo	= 2,	// 信息节点

	} RENDER_NODE_TYPE_E;

	// 渲染节点
	typedef struct _render_node
	{
		// 节点类型
		RENDER_NODE_TYPE_E enType;

		// 节点信息
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

		// 操作镜像缓存
		bool Push(PRENDER_NODE pNode);
		void Pop(PRENDER_NODE pNode);

		// 同步顶点缓存
		void Flush();

		// 清空镜像缓存
		void Clear();

	public:
		inline PDIRECT3DVERTEXBUFFER9 GetIBuffer() { return m_pIVertexBuffer; }

	private:
		bool addQuadNode( PRENDER_NODE pNode );
		bool addLineNode( PRENDER_NODE pNode );
		
	private:
		MDirect3D*	m_pD3D;

		// 顶点对象
		PDIRECT3DVERTEXBUFFER9	m_pIVertexBuffer;
		D3DVERTEXBUFFER_DESC	m_VertexBufferDesc;

		// 顶点镜像缓存
		CRITICAL_SECTION	m_csBuffer;
		MCycleMem*			m_pBuffer;
	};
}

#endif // __MLT_DX_VERTEX_BUFFER_H__




