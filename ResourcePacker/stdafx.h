// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ �������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI �����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ���ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��


//////////////////////////////////////////////////////////////////////////

//
#include <map>
#include <hash_map>
using namespace std;
using namespace stdext;


// irrlicht
//#define _IRR_STATIC_LIB_
//#define _IRR_WCHAR_FILESYSTEM
//#include <irrlicht.h>
//using namespace irr;
//
//#ifdef _IRR_WINDOWS_
//	#pragma comment(lib, "Irrlicht.lib")
//#endif


// dxwrap
#include "..\dxwrap\dxwrap.h"
#include "..\dxwrap\d3d\direct3d.h"
using namespace mlt;

#pragma comment(lib, "..\\Debug\\dxwrap.lib")


// ���ݿ����
#include "DBOper.h"
