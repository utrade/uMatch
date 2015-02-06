#ifndef ESM_REQUEST_APPLICATION_H
#define ESM_REQUEST_APPLICATION_H

#include <quickfix/Application.h>
#include <quickfix/MessageCracker.h>
#include <quickfix/fix42/NewOrderSingle.h>
#include <quickfix/fix42/OrderCancelRequest.h>
#include <quickfix/fix42/OrderCancelReplaceRequest.h>

#include "market.h"
#include "replyApplication.h"

namespace ESM {
  /**
   * \class RequestApplication
   *
   * QuickFIX Application class to handle the following messages:
   * * NewOrderSingle
   * * OrderCancelReplaceRequest
   * * OrderCancelRequest
   *
   */
  class RequestApplication :
    public FIX::Application,
    public FIX::MessageCracker
  {
    public :
      RequestApplication( const std::string &address,
                          const std::string &port ) ;

      void onCreate(const FIX::SessionID&) {}
      void onLogon(const FIX::SessionID&) {}
      void onLogout(const FIX::SessionID&) {}
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

      void onMessage ( const FIX42::NewOrderSingle&,
                       const FIX::SessionID& );
      void onMessage ( const FIX42::OrderCancelRequest&,
                       const FIX::SessionID& );
      void onMessage ( const FIX42::OrderCancelReplaceRequest&,
                       const FIX::SessionID& );

      void readCommands() ;

#ifndef UDP_MARKET_DATA
      void setMarketDataApplication(MarketDataApplication* md)
      {
        _market.setMarketDataApplication( md );
      }
#endif

    private :
      ReplyApplication _replyApplication ;

      Market _market ;

      void reject( const FIX::SessionID &sessionId,
                   const FIX::MsgSeqNum &msgSeqNum,
                   const FIX::MsgType &msgType,
                   const int &tagNum,
                   const int &sessionRejectReason,
                   const std::string &sessionRejectReasonTxt
                   ) {}

      std::string _orderGeneratorId ;
  };
}
#endif // ESM_REQUEST_APPLICATION_H
