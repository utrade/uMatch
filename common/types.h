#ifndef UT_TYPES_H
#define UT_TYPES_H

#include <stdint.h>
#include <string>
#include <string.h>

namespace UT
{
  /**
   * 1 Byte Character Fields
   */
  typedef signed char CHAR ;
  typedef unsigned char UCHAR ;

  /**
   * 1 Byte Integer Fields
   */
  typedef int8_t INT8 ;
  typedef uint8_t UINT8 ;

  /**
   * 2 Byte Integer Fields
   */
  typedef int16_t SHORT ;
  typedef int16_t USHORT ;

  /**
   * 4 Byte Integer Fields.
   * Note : As per the 'traditional' definition, long is a 4 byte int.
   */
  typedef int32_t INT ;
  typedef uint32_t UINT ;
  typedef int32_t LONG ;
  typedef int32_t ULONG ;

  /**
   * 8 Byte Integer Fields
   */
  typedef int64_t LONGLONG ;
  typedef uint64_t ULONGLONG ;

  /**
   * A double field.
   * The union with a long allows us to easily byte swap in places
   * where we have to change endianess.
   */
  union DOUBLE {
    double value ;
    int64_t longValue ;
    DOUBLE() {}
    DOUBLE( double d ) : value( d ) {}
  } ;

  /**
   * The Price Field. Keeps changing often from LONG to LONGLONG to DOUBLE.
   * Hence keep it here so that it is automatically picked up in code.
   */
  //typedef int32_t PRICE ;
  typedef double PRICE ;
}

// Note sizes in Bits
#define UT_CHAR_SIZE 8
#define UT_UCHAR_SIZE 8
#define UT_SHORT_SIZE 16
#define UT_UINT_SIZE 32
#define UT_LONG_SIZE 32
#define UT_INT_SIZE 32
#define UT_PRICE_SIZE 32
#define UT_LONGLONG_SIZE 64
#define UT_ULONGLONG_SIZE 64
#define UT_DOUBLE_SIZE 64

#endif
