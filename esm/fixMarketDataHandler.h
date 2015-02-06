#ifndef UT_ESM_FIX_MARKET_DATA_HANDLER_H
#define UT_ESM_FIX_MARKET_DATA_HANDLER_H

#include "order.h"
#include <quickfix/Application.h>
#include <quickfix/MessageCracker.h>
#include <quickfix/Session.h>

#include <boost/thread/mutex.hpp>

namespace ESM
{
  /**
   * \class MarketDataApplication
   *
   * \brief Sends market data for the matching engine in FIX Format
   *
   */
  class MarketDataApplication :
    public FIX::Application,
    public FIX::MessageCracker
  {
    public :
      /**
       * \brief Constructor to initialize the address and port
       *        where market data is to be sent
       *
       * @param address
       * @param port
       */
      MarketDataApplication()
        : _setSessions(),
          _mutexSetSessions()
      { }
      void onCreate(const FIX::SessionID&) {}
      void onLogon(const FIX::SessionID& id);
      void onLogout(const FIX::SessionID& id);

      void toAdmin(FIX::Message&, const FIX::SessionID&) {}
      void fromAdmin( const FIX::Message&, const FIX::SessionID& )
        throw( FIX::FieldNotFound,
               FIX::IncorrectDataFormat,
               FIX::IncorrectTagValue,
               FIX::RejectLogon ) {}

      void toApp( FIX::Message&, const FIX::SessionID& )
        throw( FIX::DoNotSend ) ;
      void fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
        throw( FIX::FieldNotFound,
               FIX::IncorrectDataFormat,
               FIX::IncorrectTagValue,
               FIX::UnsupportedMessageType );

      /**
       * \brief send MarketDataSnapshot to all the connected sessions
       *
        * @param message
       */
      void send( const MarketPicture &message );

    private :
      std::set<FIX::SessionID> _setSessions;
      boost::mutex _mutexSetSessions;
  };

}


#endif
