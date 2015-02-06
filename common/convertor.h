#ifndef UT_CONVERTOR_H
#define UT_CONVERTOR_H

#include "types.h"
#include "exceptions.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <limits>

namespace UT
{
  /**
   * IntConvertor class converts from Int to string and back.
   * Use member convert for either.
   */
  struct IntConvertor
  {
    public:

      /**
       * @brief Convert from string to int.
       * @param String to be converted.
       * @param result to be returned.
       * @return true if conversion was successful.
       *         false if string was not a number.
       */
      static bool convert( const std::string& value, int64_t& result )
      {
        const char* str = value.c_str();
        bool isNegative = false;
        int64_t x = 0;

        if( *str == '-' )
        {
          isNegative = true;
          ++str;
        }

        do
        {
          const int c = *str - '0';
          if( c < 0 || 9 < c ) return false;
          x = 10 * x + c;
        } while (*++str);

        if( isNegative )
          x = -x;

        result = x;
        return true;
      }

      /**
       * @brief Convert a string to int
       * @param String to be converted.
       * @return Int value extracted from the string.
       */
      static int64_t convert( const std::string& value )
      {
        int64_t result = 0;
        if( !convert( value, result ) )
          throw UT::NumberConvertException( "Number Not Integer" ) ;
        else
          return result;
      }

      /**
       * @brief Convert a string to int
       * @param String to be converted.
       * @return Int value extracted from the string.
       */
      static int64_t toInteger( const std::string &value )
      {
        return convert( value ) ;
      }

      /**
       * @brief Convert integer to string.
       * @param the buffer where we will store the new value.
       * @param size of the buffer
       * @return The integer value that has to be converted to string.
       */
      template<class T>
        static inline char* integer_to_string( char* buf, const size_t len, T t)
        {
          const bool isNegative = t < 0;
          char* p = buf + len;

          *--p = '\0';

          if( isNegative )
          {
            if( t == (std::numeric_limits<T>::min)() )
            {
              *--p = '0' + (char)((10-t%10)%10);
              t/=10;
            }
            t = -t;
            do
            {
              *--p = '0' + (char)(t % 10);
              t /= 10;
            } while (t > 0);
            *--p = '-';
          }
          else
          {
            do
            {
              *--p = '0' + (char)(t % 10);
              t /= 10;
            } while( t > 0 );
          }
          return p;
        }


      /**
       * @brief Convert an integer to string.
       * @param The integer to be converted.
       * @return The string created from the integer.
       */
      static std::string convert( int64_t value )
      {
        // buffer is big enough for significant digits and extra digit,
        // minus and null
        char buffer[std::numeric_limits<int64_t>::digits10 + 3];
        const char* const start
          = integer_to_string<int64_t>( buffer, sizeof (buffer), value );
        return std::string( start, buffer + sizeof (buffer) - start - 1 );
      }

      /**
       * @brief Convert an integer to string.
       * @param The integer to be converted.
       * @return The string created from the integer.
       */
      static std::string toString( int64_t value )
      {
        return convert( value ) ;
      }

  };

  /**
   * UnsignedIntConvertor class converts from Unsigned Int to string and back.
   * Use member convert for either.
   */
  struct UnsignedIntConvertor
  {
    public:

      /**
       * @brief Convert from string to unsigned int.
       * @param String to be converted.
       * @param result to be returned.
       * @return true if conversion was successful.
       *         false if string was not a number.
       */
      static bool convert( const std::string& value, uint64_t& result )
      {
        const char* str = value.c_str();
        uint64_t x = 0;

        if( *str == '-' )
        {
          return false ;
        }

        do
        {
          const int c = *str - '0';
          if( c < 0 || 9 < c ) return false;
          x = 10 * x + c;
        } while (*++str);

        result = x;
        return true;
      }

      /**
       * @brief Convert a string to unsigned int
       * @param String to be converted.
       * @return unsigned Int value extracted from the string.
       */
      static uint64_t convert( const std::string& value )
      {
        uint64_t result = 0;
        if( !convert( value, result ) )
          throw UT::NumberConvertException( "Number Not An Unsigned Int" ) ;
        else
          return result;
      }

      /**
       * @brief Convert integer to string.
       * @param the buffer where we will store the new value.
       * @param size of the buffer
       * @return The integer value that has to be converted to string.
       */
      template<class T>
        static inline char* integer_to_string( char* buf, const size_t len, T t)
        {
          if( t < 0 )
            throw UT::NumberConvertException( "Number is negative" ) ;

          char* p = buf + len;

          *--p = '\0';

          do
          {
            *--p = '0' + (char)(t % 10);
            t /= 10;
          } while( t > 0 );

          return p;
        }


      /**
       * @brief Convert an integer to string.
       * @param The integer to be converted.
       * @return The string created from the integer.
       */
      static std::string convert( uint64_t value )
      {
        // buffer is big enough for significant digits and extra digit,
        // minus and null
        char buffer[std::numeric_limits<uint64_t>::digits10 + 3];
        const char* const start
          = integer_to_string<uint64_t>( buffer, sizeof (buffer), value );
        return std::string( start, buffer + sizeof (buffer) - start - 1 );
      }
  };

  /// Converts double to/from a string
  struct DoubleConvertor
  {
    /**
     * @brief Convert a double to a string.
     * @param The double value to be converted.
     * @param The size of the padding required.
     * @return The string version of the double
     */
    static std::string convert( double value, int padding = 0 )
    {
      char result[32];
      char *end = 0;

      int size;
      if( value == 0 || value > 0.0001 || value <= -0.0001 )
      {
        size = sprintf( result, "%.15g", value );

        if( padding > 0 )
        {
          char* point = result;
          end = result + size - 1;
          while( *point != '.' && *point != 0 )
            point++;

          if( *point == 0 )
          {
            end = point;
            *point = '.';
            size++;
          }
          int needed = padding - (int)(end - point);

          while( needed-- > 0 )
          {
            *(++end) = '0';
            size++;
          }
          *(end+1) = 0;
        }
      }
      else
      {
        size = sprintf( result, "%.15f", value );
        // strip trailing 0's
        end = result + size - 1;

        if( padding > 0 )
        {
          int discard = 15 - padding;

          while( (*end == '0') && (discard-- > 0) )
          {
            *(end--) = 0;
            size--;
          }
        }
        else
        {
          while( *end == '0' )
          {
            *(end--) = 0;
            size--;
          }
        }
      }

      return std::string( result, size );
    }

    /**
     * @brief Convert a string to double.
     * @param The string to be converted.
     * @param The double extracted as an output variable.
     * @return True if conversion was successful.
     *         False otherwise.
     */
    static bool convert( const std::string& value, double& result )
    {
      const char * i = value.c_str();

      // Catch null strings
      if( !*i ) return false;
      // Eat leading '-' and recheck for null string
      if( *i == '-' && !*++i ) return false;

      bool haveDigit = false;

      if( isdigit(*i) )
      {
        haveDigit = true;
        while( isdigit (*++i) );
      }

      if( *i == '.' && isdigit(*++i) )
      {
        haveDigit = true;
        while( isdigit (*++i) );
      }

      if( *i || !haveDigit ) return false;
      result = strtod( value.c_str(), 0 );
      return true;
    }

    /**
     * @brief Converts a string to a double.
     * @param string to be converted.
     * @return double value extracted from the string
     */
    static double convert( const std::string& value )
    {
      double result = 0.0;
      if( !convert( value, result ) )
        throw UT::NumberConvertException( "Number Not a Decimal" ) ;
      else
        return result;
    }
  };
}
#endif
