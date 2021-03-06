//
//  s4pubtypes.h
//  S4
//
//  Created by vincent Moscaritolo on 11/2/15.
//  Copyright © 2015 4th-A Technologies, LLC. All rights reserved.
//

#ifndef s4pubtypes_h
#define s4pubtypes_h



#include <limits.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <time.h>

#ifdef __GNUC__
#define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func) func
#endif


#if ( DEBUG == 1 )
#define STATUS_LOG(...)	 printf(__VA_ARGS__)
#else
#define STATUS_LOG(...)
#endif

#define kEnumMaxValue		INT_MAX

#define ENUM_FORCE( enumName )		\
k ## enumName ## force = kEnumMaxValue

#if INT_MAX == 0x7FFFFFFFL
#define ENUM_TYPEDEF( enumName, typeName )	typedef enum enumName typeName
#else
#define ENUM_TYPEDEF( enumName, typeName )	typedef int32_t typeName
#endif

#ifndef MAX
#define MAX(a,b) (a >= b ? a : b)
#endif

#define IsS4Err(_err_)  (_err_ != kS4Err_NoErr)
#define IsntS4Err(_err_)  (_err_ == kS4Err_NoErr)

#define CKERR  if((err != kS4Err_NoErr)) {\
STATUS_LOG("ERROR %d  %s:%d \n",  err, __FILE__, __LINE__); \
goto done; }

#define ASSERTERR( _a_ , _err_ )  if(!(_a_))  { \
err = _err_; \
STATUS_LOG("ERROR %d  %s:%d \n",  err, __FILE__, __LINE__); \
goto done; }


#ifndef IsntNull
#define IsntNull( p )	( (int) ( (p) != NULL ) )
#endif


#ifndef IsNull
#define IsNull( p )		( (int) ( (p) == NULL ) )
#endif

#define RETERR(x)	do { err = x; goto done; } while(0)

#define COPY(b1, b2, len)							\
memcpy((void *)(b2), (void *)b1, (int)(len) )

static void * (* const volatile __memset_vp)(void *, int, size_t) = (memset);

#define ZERO(b1, len) \
(*__memset_vp)((void *)(b1), 0, (int)(len) )

#ifndef XMALLOC
#ifdef malloc
#define LTC_NO_PROTOTYPES
#endif
#define XMALLOC  malloc
#endif
#ifndef XREALLOC
#ifdef realloc
#define LTC_NO_PROTOTYPES
#endif
#define XREALLOC realloc
#endif
#ifndef XFREE
#ifdef free
#define LTC_NO_PROTOTYPES
#endif
#define XFREE    free
#endif

#define CMP(b1, b2, length)							\
(memcmp((void *)(b1), (void *)(b2), (length)) == 0)

#define CMP2(b1, l1, b2, l2)							\
(((l1) == (l2)) && (memcmp((void *)(b1), (void *)(b2), (l1)) == 0))

#define CKNULL(_p) if(IsNull(_p)) {\
err = kS4Err_OutOfMemory; \
goto done; }

#define BOOLVAL(x) (!(!(x)))

#define BitSet(arg,val) ((arg) |= (val))
#define BitClr(arg,val) ((arg) &= ~(val))
#define BitFlp(arg,val) ((arg) ^= (val))
#define BitTst(arg,val) BOOLVAL((arg) & (val))

#define ValidateParam( expr )	\
if ( ! (expr ) )	\
{\
STATUS_LOG("ERROR %s(%d): %s is not true\n",  __FILE__, __LINE__, #expr ); \
return( kS4Err_BadParams );\
};

#define ValidatePtr( ptr )	\
ValidateParam( (ptr) != NULL )


enum S4Err
{
    kS4Err_NoErr = 0,
    kS4Err_NOP,						// 1
    kS4Err_UnknownError,			// 2
    kS4Err_BadParams,				// 3
    kS4Err_OutOfMemory,				// 4
    kS4Err_BufferTooSmall,			// 5
    
    kS4Err_UserAbort,				// 6
    kS4Err_UnknownRequest,			// 7
    kS4Err_LazyProgrammer,			// 8
    
    kS4Err_AssertFailed,            // 9
    
    kS4Err_FeatureNotAvailable,     // 10
    kS4Err_ResourceUnavailable,     // 11
    kS4Err_NotConnected,            // 12
    kS4Err_ImproperInitialization,	// 13
    kS4Err_CorruptData,				// 14
    kS4Err_SelfTestFailed,			// 15
    kS4Err_BadIntegrity,            // 16
    kS4Err_BadHashNumber,			// 17
    kS4Err_BadCipherNumber,			// 18
    kS4Err_BadPRNGNumber,			// 19
    
    kS4Err_SecretsMismatch,			// 20
    kS4Err_KeyNotFound,				// 21
    
    kS4Err_ProtocolError,			// 22
    kS4Err_ProtocolContention,		// 23
    
    kS4Err_KeyLocked,				// 24
    kS4Err_KeyExpired,				// 25
    
    kS4Err_EndOfIteration,			// 26
    kS4Err_OtherError,				// 27
    kS4Err_PubPrivKeyNotFound,		// 28
    
    kS4Err_NotEnoughShares,         // 29

    kS4Err_PropertyNotFound,        // 30
};

typedef int S4Err;

#endif /* s4pubtypes_h */
