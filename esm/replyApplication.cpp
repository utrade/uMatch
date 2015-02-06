#include <quickfix/fix42/ExecutionReport.h>
#include <quickfix/fix42/OrderCancelReject.h>
#include <quickfix/Session.h>

#include "replyApplication.h"
#include "constants.h"
#include "fixToOrder.h"

namespace ESM {
  void ReplyApplication::sendNewConfirm( OrderPtr order )
  {
    //DEBUG_1( "New confirm " ) ;

    FIX42::ExecutionReport fixReport ( FIX::OrderID ( order->getOrderId() ),
                                      FIX::ExecID ( "1" ),
                                      FIX::ExecTransType ( FIX::ExecTransType_NEW ),
                                      FIX::ExecType ( FIX::ExecType_NEW ),
                                      FIX::OrdStatus ( FIX::OrdStatus_NEW ),
                                      FIX::Symbol ( order->getSecurityId () ),
                                      ToFix::convert( order->getSide() ),
                                      FIX::LeavesQty ( order->getActualPendingQty() ),
                                      FIX::CumQty ( order->getFilledQty() ),
                                      FIX::AvgPx ( order->getAvgPrice() )
                                      );
    fixReport.set( FIX::TransactTime() ) ;
    fixReport.set( FIX::ClOrdID( order->getClientOrderId() ) ) ;
    fixReport.set( FIX::SecurityID( order->getSecurityId () ) ) ;

    FIX::SessionID session ;
    session.fromString( order->getSenderId() ) ;
    FIX::Session::sendToTarget ( fixReport, session ) ;
  }

  void ReplyApplication::sendReplaceConfirm( OrderPtr order )
  {
    //DEBUG_1( "Replace confirm " ) ;

    FIX42::ExecutionReport fixReport( FIX::OrderID ( order->getOrderId() ),
                                      FIX::ExecID ( "1" ),
                                      FIX::ExecTransType ( FIX::ExecTransType_NEW ),
                                      FIX::ExecType ( FIX::ExecType_REPLACE ),
                                      FIX::OrdStatus ( FIX::OrdStatus_REPLACED ),
                                      FIX::Symbol ( order->getSecurityId () ),
                                      ToFix::convert( order->getSide() ),
                                      FIX::LeavesQty ( order->getActualPendingQty() ),
                                      FIX::CumQty ( order->getFilledQty() ),
                                      FIX::AvgPx ( order->getAvgPrice() )
                                      );
    fixReport.set( FIX::TransactTime() ) ;
    fixReport.set( FIX::ClOrdID( order->getClientOrderId() ) ) ;
    fixReport.set( FIX::SecurityID( order->getSecurityId () ) ) ;

    FIX::SessionID session ;
    session.fromString( order->getSenderId() ) ;
    FIX::Session::sendToTarget ( fixReport, session ) ;
  }

  void ReplyApplication::sendCancelConfirm( OrderPtr order,
                                            const std::string &reason )
  {
    //DEBUG_1( "cancel confirm " ) ;
    FIX42::ExecutionReport fixReport ( FIX::OrderID ( order->getOrderId() ),
                                      FIX::ExecID ( "1" ),
                                      FIX::ExecTransType ( FIX::ExecTransType_NEW ),
                                      FIX::ExecType ( FIX::ExecType_CANCELED ),
                                      FIX::OrdStatus ( FIX::OrdStatus_CANCELED ),
                                      FIX::Symbol ( order->getSecurityId () ),
                                      ToFix::convert( order->getSide() ),
                                      FIX::LeavesQty ( order->getActualPendingQty() ),
                                      FIX::CumQty ( order->getFilledQty() ),
                                      FIX::AvgPx ( order->getAvgPrice() )
                                      );
    fixReport.set( FIX::TransactTime() ) ;
    fixReport.set( FIX::ClOrdID( order->getClientOrderId() ) ) ;
    fixReport.set( FIX::SecurityID( order->getSecurityId () ) ) ;

    if( reason != "" )
    {
      fixReport.set( FIX::Text( reason ) ) ;
    }

    FIX::SessionID session ;
    session.fromString( order->getSenderId() ) ;
    FIX::Session::sendToTarget ( fixReport, session ) ;
  }


  void ReplyApplication::sendNewReject( OrderPtr order,
                                        const std::string &reason )
  {
    DEBUG_2( "New reject ", reason ) ;

    FIX42::ExecutionReport fixReport ( FIX::OrderID ( order->getOrderId() ),
                                      FIX::ExecID ( "1" ),
                                      FIX::ExecTransType ( FIX::ExecTransType_NEW ),
                                      FIX::ExecType ( FIX::ExecType_NEW ),
                                      FIX::OrdStatus ( FIX::OrdStatus_REJECTED ),
                                      FIX::Symbol ( order->getSecurityId () ),
                                      ToFix::convert( order->getSide() ),
                                      FIX::LeavesQty ( order->getActualPendingQty() ),
                                      FIX::CumQty ( order->getFilledQty() ),
                                      FIX::AvgPx ( order->getAvgPrice() )
                                      );
    fixReport.set( FIX::TransactTime() ) ;
    fixReport.set( FIX::ClOrdID( order->getClientOrderId() ) ) ;
    fixReport.set( FIX::SecurityID( order->getSecurityId () ) ) ;
    fixReport.set( FIX::Text( reason ) ) ;

    FIX::SessionID session ;
    session.fromString( order->getSenderId() ) ;
    FIX::Session::sendToTarget ( fixReport, session ) ;
  }

  void ReplyApplication::sendReplaceReject( OrderPtr order,
                                            const std::string &reason )
  {
    // DEBUG_2( "Replace reject", reason ) ;
    FIX42::OrderCancelReject replaceReject (
                                            order->getOrderId() ,
                                            order->getClientOrderId(),
                                            order->getOriginalClientOrderId(),
                                            FIX::OrdStatus ( FIX::OrdStatus_CALCULATED ),
                                            FIX::CxlRejResponseTo ( FIX::CxlRejResponseTo_ORDER_CANCEL_REPLACE_REQUEST ) ) ;

    replaceReject.set ( FIX::Text ( reason ) ) ;

    FIX::SessionID session ;
    session.fromString( order->getSenderId() ) ;
    FIX::Session::sendToTarget ( replaceReject, session ) ;
  }

  void ReplyApplication::sendCancelReject( OrderPtr order,
                                           const std::string &reason )
  {
    // DEBUG_2( "Cancel reject ", reason  ) ;
    FIX42::OrderCancelReject cancelReject (
                                            order->getOrderId() ,
                                            order->getClientOrderId(),
                                            order->getOriginalClientOrderId(),
                                            FIX::OrdStatus ( FIX::OrdStatus_CALCULATED ),
                                            FIX::CxlRejResponseTo ( FIX::CxlRejResponseTo_ORDER_CANCEL_REQUEST ) ) ;

    cancelReject.set ( FIX::Text ( reason ) ) ;

    FIX::SessionID session ;
    session.fromString( order->getSenderId() ) ;
    FIX::Session::sendToTarget ( cancelReject, session ) ;
  }


  void ReplyApplication::sendMarketToLimit( OrderPtr order )
  {
    //DEBUG_1( "Market to limit ") ;
    FIX42::ExecutionReport fixReport ( FIX::OrderID ( order->getOrderId() ),
                                      FIX::ExecID ( "1" ),
                                      FIX::ExecTransType ( FIX::ExecTransType_NEW ),
                                      FIX::ExecType ( FIX::ExecType_RESTATED ),
                                      FIX::OrdStatus ( FIX::OrdStatus_NEW ),
                                      FIX::Symbol ( order->getSecurityId () ),
                                      ToFix::convert( order->getSide() ),
                                      FIX::LeavesQty ( order->getActualPendingQty() ),
                                      FIX::CumQty ( order->getFilledQty() ),
                                      FIX::AvgPx ( order->getAvgPrice() )
                                      );
    fixReport.set( FIX::TransactTime() ) ;
    fixReport.set( FIX::ClOrdID( order->getClientOrderId() ) ) ;
    fixReport.set( FIX::SecurityID( order->getSecurityId () ) ) ;
    fixReport.set( FIX::Price( order->getPrice() ) ) ;

    FIX::SessionID session ;
    session.fromString( order->getSenderId() ) ;
    FIX::Session::sendToTarget ( fixReport, session ) ;

  }

  void ReplyApplication::sendTriggered( OrderPtr order )
  {
    //DEBUG_1( "Triggered" ) ;
    FIX42::ExecutionReport fixReport ( FIX::OrderID ( order->getOrderId() ),
                                      FIX::ExecID ( "1" ),
                                      FIX::ExecTransType ( FIX::ExecTransType_NEW ),
                                      FIX::ExecType ( FIX_ExecType_TRIGGERED ),
                                      FIX::OrdStatus ( FIX::OrdStatus_NEW ),
                                      FIX::Symbol ( order->getSecurityId () ),
                                      ToFix::convert( order->getSide() ),
                                      FIX::LeavesQty ( order->getActualPendingQty() ),
                                      FIX::CumQty ( order->getFilledQty() ),
                                      FIX::AvgPx ( order->getAvgPrice() )
                                      );
    fixReport.set( FIX::TransactTime() ) ;
    fixReport.set( FIX::ClOrdID( order->getClientOrderId() ) ) ;
    fixReport.set( FIX::SecurityID( order->getSecurityId () ) ) ;

    FIX::SessionID session ;
    session.fromString( order->getSenderId() ) ;
    FIX::Session::sendToTarget ( fixReport, session ) ;

  }

  void ReplyApplication::sendFillConfirm( OrderPtr order )
  {
    //DEBUG_1( "Fill confirm " ) ;
    FIX::ExecType lExecType ;
    FIX::OrdStatus lOrdStatus ;
    if( order->getPendingQty() == 0 )
    {
      lExecType = FIX::ExecType_FILL ;
      lOrdStatus = FIX::OrdStatus_FILLED ;
    }
    else
    {
      lExecType = FIX::ExecType_PARTIAL_FILL ;
      lOrdStatus = FIX::OrdStatus_PARTIALLY_FILLED ;
    }

    FIX42::ExecutionReport fixReport ( FIX::OrderID ( order->getOrderId() ),
                                      FIX::ExecID ( "1" ),
                                      FIX::ExecTransType ( FIX::ExecTransType_NEW ),
                                      lExecType,
                                      lOrdStatus,
                                      FIX::Symbol ( order->getSecurityId () ),
                                      ToFix::convert( order->getSide() ),
                                      FIX::LeavesQty ( order->getActualPendingQty() ),
                                      FIX::CumQty ( order->getFilledQty() ),
                                      FIX::AvgPx ( order->getAvgPrice() )
                                      );
    fixReport.set( FIX::TransactTime() ) ;
    fixReport.set( FIX::ClOrdID( order->getClientOrderId() ) ) ;
    fixReport.set( FIX::SecurityID( order->getSecurityId () ) ) ;
    fixReport.set( FIX::LastShares( order->getLastShares() ) ) ;
    fixReport.set( FIX::LastPx( order->getLastPrice() ) ) ;

    FIX::SessionID session ;
    session.fromString( order->getSenderId() ) ;
    FIX::Session::sendToTarget ( fixReport, session ) ;
  }

}
