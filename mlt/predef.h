/******************************************************************************
*	filename:	core.h
*	created:	11/22/2010
*	author:		Loser
*
*	purpose:	�궨��
******************************************************************************/

#ifndef __MLT_PRE_DEFINE_H__
#define __MLT_PRE_DEFINE_H__

namespace mlt
{
	/**************************************************************************
	*
	**************************************************************************/

	//
	typedef unsigned char 			byte;
	typedef unsigned short			ushort;
	typedef unsigned int			uint;
	typedef unsigned long			ulong;
	typedef unsigned __int64		uint64;

	typedef unsigned __int8			uint8;
	typedef unsigned __int16		uint16;
	typedef unsigned __int32		uint32;

	//
	typedef void*					pvoid;
	typedef short*					pshort;
	typedef int*					pint;
	typedef long*					plong;
	typedef __int64*				pint64;

	typedef __int8*					pint8;
	typedef __int16*				pint16;
	typedef __int32*				pint32;

	//
	typedef const void*				pcvoid;
	typedef const short*			pcshort;
	typedef const int*				pcint;
	typedef const long*				pclong;
	typedef const __int64*			pcint64;

	typedef const __int8*			pcint8;
	typedef const __int16*			pcint16;
	typedef const __int32*			pcint32;

	//
	typedef unsigned char*			pbyte;
	typedef unsigned short*			pushort;
	typedef unsigned int*			puint;
	typedef unsigned long*			pulong;
	typedef unsigned __int64*		puint64;

	typedef unsigned __int8*		puint8;
	typedef unsigned __int16*		puint16;
	typedef unsigned __int32*		puint32;

	// �ַ���
	#ifdef _UNICODE
		typedef wchar_t*			pstr;
		typedef const wchar_t*		pcstr;
		#define L(x)				L##x
	#else
		typedef char*				pstr;
		typedef const char*			pcstr;
		#define L(x)				x
	#endif

	//
	#ifdef	MLTE_EXPORTS
		#define	DLL_CLASS	class __declspec(dllexport)
		#define	DLL_FUNC	__declspec(dllexport)
	#else
		#define	DLL_CLASS	class __declspec(dllimport)
		#define	DLL_FUNC	__declspec(dllimport)
	#endif

	//
	#define IN
	#define OUT

	#define NULL	0
	#define DELETE_PTR(ptr)		if (ptr) { delete ptr; ptr = NULL; }
	#define DELETE_ARY(ptr)		if (ptr) { delete [] ptr; ptr = NULL; }


	/**
	*	�������� ���д��Ϊ ��
	*	void spin_lock( long volatile *p  )
	*	{
	*		while( InterlockedCompareExchange(p, 1, 0) ) 
	*		{
	*			Sleep(0);
	*		}
	*	}
	*	unlock������Ҫѭ������InterlockedExchange ��1����0�����ˡ�
	*
	***************************************************************************
	*
	*		дһ�����������뺯����һ�������ж�1��ȫ�ֱ����Ƿ�0��������ֵΪ�������߱��1��n������
	*	��0���л���������ڹ��������˳�ʲô�����ɣ�����һֱ�ȵ��ñ���Ϊ0������ñ���Ϊ0��Ѹ�
	*	������ֵΪ�����߱�ţ������������ж���ֵ�Ƿ�Ϊ�ո���ֵ��������ǣ�ѭ����һ����ĵȴ�ʱ
	*	�����1���ӣ���ʱ����������ǣ��������У����н�������0�������������ִ��ǰ�����øú�����
	*	�ṩ���Բ�ͬ�ĵ����߱��1��n��
	*
	***************************************************************************
	*
	*	�����������������������������������⣺
	*	1  CPU���棬������Ĵ�������޷�ִ�С�����㲻��volatile����Ա����ĸĶ���������CPU�����У����Ը���û�ж��������ڴ���иĶ���
	*	2  �Ķ���һ�£��������lockǰ׺�Ե�ַ���иĶ�����ô���޷�����ԭ���ԣ�����ͬʱ�Ķ����������Ԥ�ϡ�
	*	���ԣ���ϸ����4¥�ظ���volatile��interlocked�Ǳ���ģ����ǿ�ѡ�ġ�
	*/
	#define MUTEX_INIT(busy)		{ busy = 0; srand((unsigned)time(NULL)); }
	#define	MUTEX_LOCK(busy)		{ int id = rand(); do { while (busy) _sleep(1); busy = id; } while (id != busy); }
	#define MUTEX_UNLOCK(busy)		{ busy = 0; }

	// ���·������
	#define MAX_PATH_LEN	256
	// ������ֳ���
	#define MAX_NAME_LEN	64




}

#endif	// __MLT_PRE_DEFINE_H__





















