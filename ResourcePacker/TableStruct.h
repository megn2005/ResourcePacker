#pragma once


/**************************************************************************
*
**************************************************************************/

// 文件夹层次表
SQLITE_TABLE_BEGIN(folder_level)
	SQLITE_FIELD(id, integer, primary key autoincrement)	// ID
	SQLITE_FIELD(name, text, not null)						// 名称
	SQLITE_FIELD(parent_id, integer, not null)				// 父节点ID
SQLITE_TABLE_END()

// 纹理信息表
SQLITE_TABLE_BEGIN(texture_info)
	SQLITE_FIELD(id, integer, primary key autoincrement)	// ID
	SQLITE_FIELD(name, text, not null)						// 名称
	SQLITE_FIELD(folder_id, integer, not null)				// 文件夹ID
	SQLITE_FIELD(width, integer, default 0)					// 纹理宽
	SQLITE_FIELD(height, integer, default 0)				// 纹理高
	SQLITE_FIELD(color_format, integer, not null)			// 颜色格式
	SQLITE_FIELD(compressed, integer, default 0)			// 是否压缩
	SQLITE_FIELD(size, integer, default 0)					// 数据大小
	SQLITE_FIELD(data, blob, null)							// 数据
SQLITE_TABLE_END()

// 动画信息表
SQLITE_TABLE_BEGIN(anime_info)
	SQLITE_FIELD(texture_id, integer, primary key)			// 纹理ID
	SQLITE_FIELD(frame_width, integer, default 0)			// 帧宽
	SQLITE_FIELD(frame_height, integer, default 0)			// 帧高
	SQLITE_FIELD(delay, integer, default 0)					// 帧延迟
SQLITE_TABLE_END()

// 三维模型表
SQLITE_TABLE_BEGIN(model_info)
	SQLITE_FIELD(id, integer, primary key autoincrement)	// ID
	SQLITE_FIELD(name, text, not null)						// 名称
	SQLITE_FIELD(folder_id, integer, not null)				// 文件夹ID
	SQLITE_FIELD(size, integer, default 0)					// 数据大小
	SQLITE_FIELD(data, blob, null)							// 数据
SQLITE_TABLE_END()

// 模型纹理表
SQLITE_TABLE_BEGIN(model_texture)
	SQLITE_FIELD(id, integer, primary key autoincrement)	// ID
	SQLITE_FIELD(model_id, integer, not null)				// 模型ID
	SQLITE_FIELD(texture_id, integer, not null)				// 纹理ID
SQLITE_TABLE_END()


/**************************************************************************
*
**************************************************************************/

// 资源类型
enum 
{
	rtUnknown		= 0,	// 未知
	rtTexture		= 1,	// 纹理图片
	rt3DModel		= 2,	// 3维模型
	rtOther			= 3,	// 其它
};


// 文件夹信息
class CFolderInfo
{
public:
	// 构造函数
	CFolderInfo()
		: dwId(0), dwParentId(-1), bLoaded(FALSE)
	{
	}

public:
	// ID
	DWORD		dwId;
	// 名称
	CString		strName;
	// 父节点ID
	DWORD		dwParentId;

public:
	// 加载标记
	BOOL		bLoaded;
};


// 资源基本信息
class CResourceInfo
{
public:
	// 构造函数
	CResourceInfo()
		: dwId(0), btType(rtUnknown), dwFolderId(-1), 
		u64Size(0), pData(NULL)
	{
	}
	CResourceInfo(BYTE btType)
		: dwId(0), btType(btType), dwFolderId(-1), 
		u64Size(0), pData(NULL)
	{
	}

public:
	// ID
	DWORD		dwId;
	// 类型
	BYTE		btType;	
	// 名称
	CString		strName;
	// 文件夹ID
	DWORD		dwFolderId;

	// 大小
	UINT64		u64Size;
	// 数据
	LPVOID		pData;
};


// 纹理颜色格式
enum
{
	TEXFMT_UNKNOWN		= 0,
	TEXFMT_R8G8B8		= 20,
	TEXFMT_A8R8G8B8		= 21,
	TEXFMT_X8R8G8B8		= 22,
	TEXFMT_R5G6B5		= 23,
	TEXFMT_X1R5G5B5		= 24,
	TEXFMT_A1R5G5B5		= 25,
	TEXFMT_A8B8G8R8		= 32,
	TEXFMT_X8B8G8R8		= 33
};

// 纹理信息
class CTextureInfo : public CResourceInfo
{
public:
	// 构造函数
	CTextureInfo() : CResourceInfo(rtTexture), 
		dwWidth(0), dwHeight(0), dwClrFormat(0), bCompressed(FALSE)
	{
	}

public:
	// 纹理宽
	DWORD		dwWidth;
	// 纹理高
	DWORD		dwHeight;
	// 颜色格式
	DWORD		dwClrFormat;
	// 是否压缩
	BOOL		bCompressed;
};

// 动画信息
class CAnimeInfo
{
public:
	// 构造函数
	CAnimeInfo() 
		: dwFrameW(0), dwFrameH(0), dwDelay(0)
	{
	}

public:
	// 纹理信息
	CTextureInfo	ti;

	// 帧宽
	DWORD		dwFrameW;
	// 帧高
	DWORD		dwFrameH;
	// 帧延迟
	DWORD		dwDelay;
};


// 模型信息
class CModelInfo : public CResourceInfo
{
public:
	// 构造函数
	CModelInfo() : CResourceInfo(rt3DModel)
	{
	}

public:
	// 贴图纹理
	vector<CTextureInfo>	vecTextureInfos;
};






