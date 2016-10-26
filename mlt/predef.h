/******************************************************************************
*	filename:	core.h
*	created:	11/22/2010
*	author:		Loser
*
*	purpose:	宏定义
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

	// 字符集
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
	*	自旋锁， 最简单写法为 ：
	*	void spin_lock( long volatile *p  )
	*	{
	*		while( InterlockedCompareExchange(p, 1, 0) ) 
	*		{
	*			Sleep(0);
	*		}
	*	}
	*	unlock，不需要循环，用InterlockedExchange 把1换成0就行了。
	*
	***************************************************************************
	*
	*		写一个函数，进入函数第一件事是判断1个全局变量是否＞0（假设其值为‘调用者编号1～n’），
	*	＞0则有互斥对象正在工作），退出什么都不干；或者一直等到该变量为0。如果该变量为0则把该
	*	变量赋值为调用者编号，并立即读出判断其值是否为刚赋的值，如果不是，循环等一段最长的等待时
	*	间比如1分钟，超时放弃。如果是，继续运行，运行结束后置0。两个互斥对象执行前都调用该函数并
	*	提供各自不同的调用者编号1～n。
	*
	***************************************************************************
	*
	*	自旋变量必须用总线锁，否则有两点问题：
	*	1  CPU缓存，导致你的代码根本无法执行。如果你不加volatile，你对变量的改动多数是在CPU缓存中，所以根本没有对真正的内存进行改动。
	*	2  改动不一致，如果不用lock前缀对地址进行改动，那么便无法做到原子性，存在同时改动，结果难以预料。
	*	所以，仔细看我4楼回复，volatile和interlocked是必须的，不是可选的。
	*/
	#define MUTEX_INIT(busy)		{ busy = 0; srand((unsigned)time(NULL)); }
	#define	MUTEX_LOCK(busy)		{ int id = rand(); do { while (busy) _sleep(1); busy = id; } while (id != busy); }
	#define MUTEX_UNLOCK(busy)		{ busy = 0; }

	// 最大路径长度
	#define MAX_PATH_LEN	256
	// 最大名字长度
	#define MAX_NAME_LEN	64




}

#endif	// __MLT_PRE_DEFINE_H__





















