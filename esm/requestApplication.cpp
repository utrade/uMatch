
#include <quickfix/fix42/Reject.h>
#include "requestApplication.h"
#include "fixToOrder.h"
#include <dismantleFix.h>

namespace ESM {
  RequestApplication::RequestApplication( const std::string &address,
                                          const std::string &port )
    : _market( _replyApplication, address, port ),
      _orderGeneratorId( "orderGenerator" )
  {
  }

  void RequestApplication::toApp( FIX::Message&message, const FIX::SessionID& )
    throw( FIX::DoNotSend )
  {
#ifndef NDEBUG
    if(  message.getHeader().getField( FIX::FIELD::TargetCompID )
            .find( _orderGeneratorId ) == std::string::npos  )
    {
      UT::DismantleFix::dismantle( "OUTBOUND", message.toString() ) ;
    }
#endif
  }

  void RequestApplication::fromApp(const FIX::Message& message,
                                   const FIX::SessionID& sessionId )
    throw( FIX::FieldNotFound,
           FIX::IncorrectDataFormat,
           FIX::IncorrectTagValue,
           FIX::UnsupportedMessageType )

  {
#ifndef NDEBUG
    if ( message.getHeader().getField( FIX::FIELD::SenderCompID )
            .find( _orderGeneratorId ) == std::string::npos )
    {
      UT::DismantleFix::dismantle( "INBOUND", message.toString() ) ;
    }
#endif

    try
    {
      crack( message, sessionId );
    }
    catch( std::exception &e )
    {
      FIX42::Reject fixReject;
      fixReject.setField( FIX::FIELD::RefSeqNum,
          message.getHeader().getField( FIX::FIELD::MsgSeqNum ) ) ;

      fixReject.setField( FIX::FIELD::RefMsgType,
          message.getHeader().getField( FIX::FIELD::MsgType ) ) ;

      fixReject.setField( FIX::FIELD::Text, e.what() ) ;

      try
      {
        FIX::Session::sendToTarget ( fixReject, sessionId ) ;

      }
      catch ( FIX::SessionNotFound &e )
      {
        std::cout << "Error on Sending execution report "
                  << e.what() << std::endl ;
      }
    }
  }

  void RequestApplication::onMessage (
      const FIX42::NewOrderSingle &newOrder,
      const FIX::SessionID &sessionId )
  {
    FIX::Side lSide ;
    newOrder.getField( lSide ) ;

    FIX::OrdType lOrdType ;
    newOrder.getField( lOrdType ) ;

    FIX::OrderQty lOrderQty ;
    newOrder.getField( lOrderQty ) ;

    NewOrderPtr order(
        new NewOrder( newOrder.getField( FIX::FIELD::SecurityID ),
          newOrder.getField( FIX::FIELD::ClOrdID ),
          sessionId.toString(),
          FromFix::convert( lSide ),
          FromFix::convert( lOrdType ),
          lOrderQty ) ) ;


    try
    {
      FIX::TimeInForce lTimeInForce;
      if( newOrder.isSetField( lTimeInForce ) )
      {
        newOrder.getField( lTimeInForce );
        order->setTimeInForce( FromFix::convert( lTimeInForce ) ) ;
      }
    }catch( std::exception &e ) {}

    FIX::MaxFloor lMaxFloor ;
    if( newOrder.isSetField( lMaxFloor ) )
    {
      newOrder.getField( lMaxFloor ) ;
      order->setDisclosedQty( lMaxFloor ) ;
    }

    switch( order->getOrderType() )
    {
      case OrderType_LIMIT :
        {
          FIX::Price lPrice ;
          newOrder.getField( lPrice ) ;
          order->setPrice( lPrice ) ;
        }
        break ;
      case OrderType_STOP_LIMIT :
        {
          FIX::Price lPrice ;
          newOrder.getField( lPrice ) ;
          order->setPrice( lPrice ) ;
        }
        // No break here
      case OrderType_STOP :
        {
          FIX::StopPx lStopPx ;
          newOrder.getField( lStopPx ) ;
          order->setStopPrice( lStopPx ) ;
        }
        break ;
      case OrderType_MARKET :
        break ;
    }

    _market.insert( order ) ;
  }

  void RequestApplication::onMessage (
      const FIX42::OrderCancelRequest &cancelOrder,
      const FIX::SessionID &sessionId )
  {
    FIX::Side lSide ;
    cancelOrder.getField( lSide ) ;

    FIX::OrdType lOrdType ;
    cancelOrder.getField( lOrdType ) ;

    FIX::OrderQty lOrderQty ;
    cancelOrder.getField( lOrderQty ) ;

    CancelOrderPtr order( new CancelOrder(
          cancelOrder.getField( FIX::FIELD::OrderID ),
          cancelOrder.getField( FIX::FIELD::OrigClOrdID ),
          cancelOrder.getField( FIX::FIELD::SecurityID ),
          cancelOrder.getField( FIX::FIELD::ClOrdID ),
          sessionId.toString(),
          FromFix::convert( lSide ),
          FromFix::convert( lOrdType ),
          lOrderQty ) ) ;



    _market.cancel( order ) ;
  }

  void RequestApplication::onMessage (
      const FIX42::OrderCancelReplaceRequest &replaceOrder,
      const FIX::SessionID &sessionId )
  {
    FIX::Side lSide ;
    replaceOrder.getField( lSide ) ;

    FIX::OrdType lOrdType ;
    replaceOrder.getField( lOrdType ) ;

    FIX::OrderQty lOrderQty ;
    replaceOrder.getField( lOrderQty ) ;

    ReplaceOrderPtr order( new ReplaceOrder(
                               replaceOrder.getField( FIX::FIELD::OrderID ),
                               replaceOrder.getField( FIX::FIELD::OrigClOrdID ),
                               replaceOrder.getField( FIX::FIELD::SecurityID ),
                               replaceOrder.getField( FIX::FIELD::ClOrdID ),
                               sessionId.toString(),
                               FromFix::convert( lSide ),
                               FromFix::convert( lOrdType ),
                               lOrderQty ) ) ;


    try
    {
      FIX::TimeInForce lTimeInForce;
      if( replaceOrder.isSetField( lTimeInForce ) )
      {
        replaceOrder.getField( lTimeInForce );
        order->setTimeInForce( FromFix::convert( lTimeInForce ) ) ;
      }
    }catch( std::exception &e ) {}

    FIX::MaxFloor lMaxFloor ;
    if( replaceOrder.isSetField( lMaxFloor ) )
    {
      replaceOrder.getField( lMaxFloor ) ;
      order->setDisclosedQty( lMaxFloor ) ;
    }

    switch( order->getOrderType() )
    {
      case OrderType_LIMIT :
        {
          FIX::Price lPrice ;
          replaceOrder.getField( lPrice ) ;
          order->setPrice( lPrice ) ;
        }
        break ;
      case OrderType_STOP_LIMIT :
        {
          FIX::Price lPrice ;
          replaceOrder.getField( lPrice ) ;
          order->setPrice( lPrice ) ;
        }
        // No break here
      case OrderType_STOP :
        {
          FIX::StopPx lStopPx ;
          replaceOrder.getField( lStopPx ) ;
          order->setStopPrice( lStopPx ) ;
        }
        break ;
      case OrderType_MARKET :
        break ;
    }

    FIX::CumQty lCumQty ;
    replaceOrder.getField( lCumQty ) ;
    order->addOrderQty( lCumQty ) ;

    _market.replace( order ) ;
  }

  void RequestApplication::readCommands()
  {
    _market.readCommands() ;
  }

}
