#pragma once


/**************************************************************************
*
**************************************************************************/

// �ļ��в�α�
SQLITE_TABLE_BEGIN(folder_level)
	SQLITE_FIELD(id, integer, primary key autoincrement)	// ID
	SQLITE_FIELD(name, text, not null)						// ����
	SQLITE_FIELD(parent_id, integer, not null)				// ���ڵ�ID
SQLITE_TABLE_END()

// ������Ϣ��
SQLITE_TABLE_BEGIN(texture_info)
	SQLITE_FIELD(id, integer, primary key autoincrement)	// ID
	SQLITE_FIELD(name, text, not null)						// ����
	SQLITE_FIELD(folder_id, integer, not null)				// �ļ���ID
	SQLITE_FIELD(width, integer, default 0)					// �����
	SQLITE_FIELD(height, integer, default 0)				// �����
	SQLITE_FIELD(color_format, integer, not null)			// ��ɫ��ʽ
	SQLITE_FIELD(compressed, integer, default 0)			// �Ƿ�ѹ��
	SQLITE_FIELD(size, integer, default 0)					// ���ݴ�С
	SQLITE_FIELD(data, blob, null)							// ����
SQLITE_TABLE_END()

// ������Ϣ��
SQLITE_TABLE_BEGIN(anime_info)
	SQLITE_FIELD(texture_id, integer, primary key)			// ����ID
	SQLITE_FIELD(frame_width, integer, default 0)			// ֡��
	SQLITE_FIELD(frame_height, integer, default 0)			// ֡��
	SQLITE_FIELD(delay, integer, default 0)					// ֡�ӳ�
SQLITE_TABLE_END()

// ��άģ�ͱ�
SQLITE_TABLE_BEGIN(model_info)
	SQLITE_FIELD(id, integer, primary key autoincrement)	// ID
	SQLITE_FIELD(name, text, not null)						// ����
	SQLITE_FIELD(folder_id, integer, not null)				// �ļ���ID
	SQLITE_FIELD(size, integer, default 0)					// ���ݴ�С
	SQLITE_FIELD(data, blob, null)							// ����
SQLITE_TABLE_END()

// ģ�������
SQLITE_TABLE_BEGIN(model_texture)
	SQLITE_FIELD(id, integer, primary key autoincrement)	// ID
	SQLITE_FIELD(model_id, integer, not null)				// ģ��ID
	SQLITE_FIELD(texture_id, integer, not null)				// ����ID
SQLITE_TABLE_END()


/**************************************************************************
*
**************************************************************************/

// ��Դ����
enum 
{
	rtUnknown		= 0,	// δ֪
	rtTexture		= 1,	// ����ͼƬ
	rt3DModel		= 2,	// 3άģ��
	rtOther			= 3,	// ����
};


// �ļ�����Ϣ
class CFolderInfo
{
public:
	// ���캯��
	CFolderInfo()
		: dwId(0), dwParentId(-1), bLoaded(FALSE)
	{
	}

public:
	// ID
	DWORD		dwId;
	// ����
	CString		strName;
	// ���ڵ�ID
	DWORD		dwParentId;

public:
	// ���ر��
	BOOL		bLoaded;
};


// ��Դ������Ϣ
class CResourceInfo
{
public:
	// ���캯��
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
	// ����
	BYTE		btType;	
	// ����
	CString		strName;
	// �ļ���ID
	DWORD		dwFolderId;

	// ��С
	UINT64		u64Size;
	// ����
	LPVOID		pData;
};


// ������ɫ��ʽ
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

// ������Ϣ
class CTextureInfo : public CResourceInfo
{
public:
	// ���캯��
	CTextureInfo() : CResourceInfo(rtTexture), 
		dwWidth(0), dwHeight(0), dwClrFormat(0), bCompressed(FALSE)
	{
	}

public:
	// �����
	DWORD		dwWidth;
	// �����
	DWORD		dwHeight;
	// ��ɫ��ʽ
	DWORD		dwClrFormat;
	// �Ƿ�ѹ��
	BOOL		bCompressed;
};

// ������Ϣ
class CAnimeInfo
{
public:
	// ���캯��
	CAnimeInfo() 
		: dwFrameW(0), dwFrameH(0), dwDelay(0)
	{
	}

public:
	// ������Ϣ
	CTextureInfo	ti;

	// ֡��
	DWORD		dwFrameW;
	// ֡��
	DWORD		dwFrameH;
	// ֡�ӳ�
	DWORD		dwDelay;
};


// ģ����Ϣ
class CModelInfo : public CResourceInfo
{
public:
	// ���캯��
	CModelInfo() : CResourceInfo(rt3DModel)
	{
	}

public:
	// ��ͼ����
	vector<CTextureInfo>	vecTextureInfos;
};






