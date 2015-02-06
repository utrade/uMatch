#ifndef ESM_FIX_TO_ORDER_H
#define ESM_FIX_TO_ORDER_H

#include <quickfix/Fields.h>
#include "structures.h"
#include "exceptions.h"

namespace ESM
{
  /**
   * @brief A class to convert from our native order format to fix. 
   */
  class ToFix
  {
    public :
      /**
       * @brief Convert the side from native to fix.
       *
       * @param the side to be converted.
       */
      static FIX::Side convert( Side side )
      {
        switch( side )
        {
          case Side_BUY :
            return FIX::Side_BUY ;
          case Side_SELL :
            return FIX::Side_SELL ;
          case Side_SELL_SHORT :
            return FIX::Side_SELL_SHORT ;
        }
        throw OrderError( "Unknown Side" ) ;
      }

      /**
       * @brief Convert the Order Type from native to fix.
       *
       * @param order type to be converted.
       */
      static FIX::OrdType convert( OrderType ordType )
      {
        switch( ordType )
        {
          case OrderType_MARKET :
            return FIX::OrdType_MARKET ;
          case OrderType_LIMIT :
            return FIX::OrdType_LIMIT ;
          case OrderType_STOP :
            return FIX::OrdType_STOP ;
          case OrderType_STOP_LIMIT:
            return FIX::OrdType_STOP_LIMIT ;
        }
        throw OrderTypeNotHandled( "" ) ;
      }

  };

  /**
   * @return Convert from fix to native type.
   */
  class FromFix
  {
    public :
      /**
       * @brief Convert the side from fix to native.
       *
       * @param The fix side to the converted.
       *
       * @return The converted side, in native format.
       */
      static Side convert( const FIX::Side &side )
      {
        switch( side )
        {
          case FIX::Side_BUY :
            return Side_BUY ;
          case FIX::Side_SELL :
            return Side_SELL ;
          case FIX::Side_SELL_SHORT:
            return Side_SELL_SHORT;
        }
        throw SideNotHandled( side.getString() ) ;
      }

      /**
       * @brief Convert the order type from fix to native.
       *
       * @param The fix order type to the converted.
       *
       * @return The converted order type, in native format.
       */
      static OrderType convert( const FIX::OrdType &ordType )
      {
        switch( ordType )
        {
          case FIX::OrdType_MARKET :
            return OrderType_MARKET ;
          case FIX::OrdType_LIMIT :
            return OrderType_LIMIT ;
          case FIX::OrdType_STOP :
            return OrderType_STOP ;
          case FIX::OrdType_STOP_LIMIT :
            return OrderType_STOP_LIMIT;
        }
        throw OrderTypeNotHandled( ordType.getString() ) ;
      }

      /**
       * @brief Convert the time in force from fix to native.
       *
       * @param The fix time in force to the converted.
       *
       * @return The converted time in force, in native format.
       */
      static TimeInForce convert( const FIX::TimeInForce &timeInForce )
      {
        switch( timeInForce )
        {
          case FIX::TimeInForce_DAY :
            return TimeInForce_DAY ;
          case FIX::TimeInForce_IMMEDIATE_OR_CANCEL :
            return TimeInForce_IOC ;
        }
        throw TimeInForceNotHandled( timeInForce.getString() ) ;
      }
  };

}

#endif // ESM_FIX_TO_ORDER_H
