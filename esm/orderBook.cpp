#include "orderBook.h"

namespace ESM
{
  void OrderBook::print()
  {
    std::cout << "============== buy ============= " << std::endl ;
    _buyOrders.print() ;
    std::cout << std::endl << "============== sell ============= " << std::endl ;
    _sellOrders.print() ;
  }

  OrderBook::OrderBook( ReplyApplication &replyApplication, OrderPtr order )
    : _replyApplication( replyApplication ),
    _hasChanged( false ),
    _isActive( true )
  {
    _marketPictureRecord.setScripCodeFromString( order->getSecurityId() ) ;

    _marketPictureRecord.setOpenPrice( order->getPrice() ) ;
    _marketPictureRecord.setClosePrice( order->getPrice() ) ;
    _marketPictureRecord.setHighPrice( order->getPrice() ) ;
    _marketPictureRecord.setLowPrice( order->getPrice() ) ;
    _marketPictureRecord.setLowerCktLimit( order->getPrice() * 9 / 10) ;
    _marketPictureRecord.setUpperCktLimit( order->getPrice() * 11 / 10) ;
  }

  void OrderBook::insert( OrderPtr order )
  {
    if( _isActive )
    {
      try
      {
        switch( order->getOrderType() )
        {
          case OrderType_MARKET :
          case OrderType_LIMIT :
            {
              switch( order->getSide() )
              {
                case Side_BUY :
                  _replyApplication.sendNewConfirm( order ) ;
                  insertBuy( order ) ;
                  break ;
                case Side_SELL_SHORT :
                case Side_SELL :
                  _replyApplication.sendNewConfirm( order ) ;
                  insertSell( order ) ;
                  break ;
                default :
                  _replyApplication.sendNewReject( order, "Unknown Side" ) ;
              }
            }
            break ;

          case OrderType_STOP :
          case OrderType_STOP_LIMIT :
            {
              switch( order->getSide() )
              {
                case Side_BUY :
                  _replyApplication.sendNewConfirm( order ) ;
                  insertStopLossBuy( order ) ;
                  break ;
                case Side_SELL_SHORT :
                case Side_SELL :
                  _replyApplication.sendNewConfirm( order ) ;
                  insertStopLossSell( order ) ;
                  break ;
                default :
                  _replyApplication.sendNewReject( order, "Unknown Side" ) ;
              }
            }
            break ;
          default:
            _replyApplication.sendNewReject( order, "Unknown Order Type" ) ;
        }
      }
      catch( std::exception &e )
      {
        _replyApplication.sendNewReject( order, e.what() ) ;
      }

      _hasChanged = true ;
    }
    else
    {
      _replyApplication.sendNewReject( order,
          "You cant place new orders as the market is closed" ) ;
    }
  }

  void OrderBook::replace( OrderPtr order )
  {
    if( _isActive )
    {
      try
      {
        switch( order->getOrderType() )
        {
          case OrderType_MARKET :
          case OrderType_LIMIT :
          case OrderType_STOP :
          case OrderType_STOP_LIMIT :
            {
              switch( order->getSide() )
              {
                case Side_BUY :
                  replaceBuy( order ) ;
                  break ;
                case Side_SELL_SHORT :
                case Side_SELL :
                  replaceSell( order ) ;
                  break ;
                default :
                  _replyApplication.sendReplaceReject( order, "Unknown Side" ) ;
              }
            }
            break ;
          default :
            _replyApplication.sendReplaceReject( order, "Unknown Order Type" ) ;
        }
      }
      catch( std::exception &e )
      {
        _replyApplication.sendReplaceReject( order, e.what() ) ;
      }
      _hasChanged = true ;
    }
    else
    {
      _replyApplication.sendNewReject( order,
          "You cant replace orders as the market is closed" ) ;
    }
  }

  void OrderBook::cancel( OrderPtr order )
  {
    try
    {
      OrderPtr canceledOrder ;

      switch( order->getOrderType() )
      {
        case OrderType_MARKET :
        case OrderType_LIMIT :
          {
            switch( order->getSide() )
            {
              case Side_BUY :
                canceledOrder = _buyOrders.cancel( order ) ;
                _replyApplication.sendCancelConfirm( canceledOrder,
                    "Order Cancelled Successfully" ) ;
                break ;
              case Side_SELL_SHORT :
              case Side_SELL :
                canceledOrder = _sellOrders.cancel( order ) ;
                _replyApplication.sendCancelConfirm( canceledOrder,
                    "Order Cancelled Successfully" ) ;
                break ;
              default :
                _replyApplication.sendCancelReject( order, "Unknown Side" ) ;
            }
          }
          break ;

        case OrderType_STOP :
        case OrderType_STOP_LIMIT :
          {
            switch( order->getSide() )
            {
              case Side_BUY :
                canceledOrder = _stopLossBuyOrders.cancel( order ) ;
                _replyApplication.sendCancelConfirm( canceledOrder,
                    "Order Cancelled Successfully" ) ;
                break ;
              case Side_SELL_SHORT :
              case Side_SELL :
                canceledOrder = _stopLossSellOrders.cancel( order ) ;
                _replyApplication.sendCancelConfirm( canceledOrder,
                    "Order Cancelled Successfully" ) ;
                break ;
              default :
                _replyApplication.sendCancelReject( order, "Unknown Side" ) ;
            }
          }
          break ;
        default:
          _replyApplication.sendCancelReject( order, "Unknown Order Type" ) ;
      }
    }
    catch( std::exception &e )
    {
      _replyApplication.sendCancelReject( order, e.what() ) ;
    }
    _hasChanged = true ;
  }

  void OrderBook::insertStopLossBuy( OrderPtr order )
  {
    if( _marketPictureRecord.getLastTradePrice() >= order->getStopPrice() )
    {
      order->trigger();
      _replyApplication.sendTriggered( order ) ;
      insertBuy( order ) ;
    }
    else
    {
      _stopLossBuyOrders.insert( order->getStopPrice(), order ) ;
    }
  }

  void OrderBook::insertStopLossSell( OrderPtr order )
  {
    if( _marketPictureRecord.getLastTradePrice() <= order->getStopPrice() )
    {
      order->trigger();
      _replyApplication.sendTriggered( order ) ;
      insertSell( order ) ;
    }
    else
    {
      _stopLossSellOrders.insert( order->getStopPrice(), order ) ;
    }
  }

  void OrderBook::replaceBuy( OrderPtr newOrder )
  {
    OrderPtr replacedOrder ;
    try
    {
      try
      {
        replacedOrder = _buyOrders.replace( newOrder ) ;
        _replyApplication.sendReplaceConfirm( replacedOrder ) ;
      }
      catch( OrderHasChanged &e )
      {
        replacedOrder = _buyOrders.erase( newOrder->getOrderId() ) ;
        _replyApplication.sendReplaceConfirm( replacedOrder ) ;

        if( replacedOrder->getOrderType() == OrderType_STOP
            || replacedOrder->getOrderType() == OrderType_STOP_LIMIT )
        {
          insertStopLossBuy( replacedOrder ) ;
        }
        else
        {
          insertBuy( replacedOrder ) ;
        }
      }
      catch( OrderIdNotFound &e )
      {
        replacedOrder = _stopLossBuyOrders.replace( newOrder );
        _replyApplication.sendReplaceConfirm( replacedOrder ) ;
      }
    }
    catch( OrderHasChanged &e )
    {
      replacedOrder = _stopLossBuyOrders.erase( newOrder->getOrderId() ) ;
      _replyApplication.sendReplaceConfirm( replacedOrder ) ;

      if( replacedOrder->getOrderType() == OrderType_STOP
          || replacedOrder->getOrderType() == OrderType_STOP_LIMIT )
        insertStopLossBuy( replacedOrder ) ;
      else
        insertBuy( replacedOrder ) ;
    }
    catch( std::exception &e )
    {
      _replyApplication.sendReplaceReject( newOrder, e.what() ) ;
    }

    if( newOrder->getTimeInForce() == TimeInForce_IOC )
    {
      OrderPtr canceledOrder = _buyOrders.cancel( newOrder ) ;
      _replyApplication.sendCancelConfirm(
        canceledOrder,
        "IOC Order Cancelled Successfully"
        ) ;
    }
  }

  void OrderBook::replaceSell( OrderPtr newOrder )
  {
    OrderPtr replacedOrder ;
    try
    {
      try
      {
        replacedOrder = _sellOrders.replace( newOrder ) ;
        _replyApplication.sendReplaceConfirm( replacedOrder ) ;
      }
      catch( OrderHasChanged &e )
      {
        replacedOrder = _sellOrders.erase( newOrder->getOrderId() ) ;
        _replyApplication.sendReplaceConfirm( replacedOrder ) ;

        if( replacedOrder->getOrderType() == OrderType_STOP
            || replacedOrder->getOrderType() == OrderType_STOP_LIMIT )
          insertStopLossSell( replacedOrder ) ;
        else
          insertSell( replacedOrder ) ;
      }
      catch( OrderIdNotFound &e )
      {
        replacedOrder = _stopLossSellOrders.replace( newOrder );
        _replyApplication.sendReplaceConfirm( replacedOrder ) ;
      }
    }
    catch( OrderHasChanged &e )
    {
      replacedOrder = _stopLossSellOrders.erase( newOrder->getOrderId() ) ;
      _replyApplication.sendReplaceConfirm( replacedOrder ) ;

      if( replacedOrder->getOrderType() == OrderType_STOP
          || replacedOrder->getOrderType() == OrderType_STOP_LIMIT )
        insertStopLossSell( replacedOrder ) ;
      else
        insertSell( replacedOrder ) ;
    }
    catch( std::exception &e )
    {
      _replyApplication.sendReplaceReject( newOrder, e.what() ) ;
    }

    if( newOrder->getTimeInForce() == TimeInForce_IOC )
    {
      OrderPtr canceledOrder = _sellOrders.cancel( newOrder ) ;
      _replyApplication.sendCancelConfirm(
        canceledOrder,
        "IOC Order Cancelled Successfully"
        ) ;
    }
  }

  void OrderBook::updateMarketData( int price, int qty )
  {
    _marketPictureRecord.setNoOfTrades( _marketPictureRecord.getNoOfTrades() + 1 ) ;
    _marketPictureRecord.setVolume( _marketPictureRecord.getVolume() + qty ) ;
    _marketPictureRecord.setValue( _marketPictureRecord.getValue() + ( qty * price ) ) ;
    _marketPictureRecord.setLastTradePrice( price ) ;
    _marketPictureRecord.setLastTradeQty( qty ) ;
    _marketPictureRecord.setWeightedAvgPrice ( _marketPictureRecord.getValue()
                                               / _marketPictureRecord.getVolume() ) ;

    if( _marketPictureRecord.getHighPrice() < price )
    {
      _marketPictureRecord.setHighPrice( price ) ;
    }
    else if( _marketPictureRecord.getLowPrice() > price )
    {
      _marketPictureRecord.setLowPrice( price ) ;
    }
  }

  void OrderBook::insertBuy( OrderPtr buyOrder )
  {
    try
    {
      boost::mutex::scoped_lock lock( _mutexOnMatch ) ;

      while( buyOrder->getPendingQty() > 0 &&
             ( buyOrder->getOrderType() == OrderType_MARKET
               || buyOrder->getPrice() >= _sellOrders.first()->getPrice() ) )
      {
        OrderPtr sellOrder = _sellOrders.first() ;

        int qty = ( buyOrder->getPendingQty() < sellOrder->getPendingQty() )
                    ? buyOrder->getPendingQty() : sellOrder->getPendingQty() ;

        _sellOrders.fill( sellOrder->getPrice(), qty ) ;
        buyOrder->fill( sellOrder->getPrice(), qty ) ;
        _replyApplication.sendFillConfirm( sellOrder ) ;
        _replyApplication.sendFillConfirm( buyOrder ) ;

        updateMarketData( sellOrder->getPrice(), qty ) ;

        checkTriggeredOrders() ;
      }
    }
    catch( ListIsEmpty &e )
    {
    }

    if( buyOrder->getPendingQty() > 0 )
    {
      if( buyOrder->getTimeInForce() == TimeInForce_IOC )
      {
        _replyApplication.sendCancelConfirm(
          buyOrder,
          "IOC Order Cancelled Successfully"
          ) ;
      }
      else
      {
        if( buyOrder->getOrderType() == OrderType_MARKET )
        {
          if( _marketPictureRecord.getLastTradePrice() > 0 )
          {
            buyOrder->setMarketToLimit( _marketPictureRecord.getLastTradePrice() ) ;
            _replyApplication.sendMarketToLimit( buyOrder ) ;
          }
          else
          {
            _replyApplication.sendCancelConfirm(
              buyOrder,
              "Market Order Cancelled since we do not have a LTP"
              ) ;
            return ;
          }
        }
        _buyOrders.insert( buyOrder->getPrice(), buyOrder ) ;
      }
    }
  }

  void OrderBook::insertSell( OrderPtr sellOrder )
  {
    try
    {
      while( sellOrder->getPendingQty() > 0
             && ( sellOrder->getOrderType() == OrderType_MARKET
                  || sellOrder->getPrice() <= _buyOrders.first()->getPrice() ) )
      {
        OrderPtr buyOrder = _buyOrders.first() ;

        int qty = ( buyOrder->getPendingQty() < sellOrder->getPendingQty() )
                    ? buyOrder->getPendingQty()
                    : sellOrder->getPendingQty() ;

        _buyOrders.fill( buyOrder->getPrice(), qty ) ;
        sellOrder->fill( buyOrder->getPrice(), qty ) ;
        _replyApplication.sendFillConfirm( buyOrder ) ;
        _replyApplication.sendFillConfirm( sellOrder ) ;

        updateMarketData( buyOrder->getPrice(), qty ) ;

        checkTriggeredOrders() ;
      }
    }
    catch( ListIsEmpty &e )
    {
    }

    if( sellOrder->getPendingQty() > 0 )
    {
      if( sellOrder->getTimeInForce() == TimeInForce_IOC )
      {
        _replyApplication.sendCancelConfirm(
          sellOrder,
          "IOC Order Cancelled Successfully"
          ) ;
      }
      else
      {
        if( sellOrder->getOrderType() == OrderType_MARKET )
        {
          if( _marketPictureRecord.getLastTradePrice() > 0 )
          {
            sellOrder->setMarketToLimit( _marketPictureRecord.getLastTradePrice() ) ;
            _replyApplication.sendMarketToLimit( sellOrder ) ;
          }
          else
          {
            _replyApplication.sendCancelConfirm(
              sellOrder,
              "Market Order Cancelled since we do not have a LTP"
              ) ;
            return ;
          }
        }
        _sellOrders.insert( sellOrder->getPrice(), sellOrder ) ;
      }
    }
  }

  void OrderBook::checkTriggeredOrders( )
  {

    try
    {
      while( _marketPictureRecord.getLastTradePrice() >=
             _stopLossBuyOrders.first()->getStopPrice() )
      {
        OrderPtr buyOrder = _stopLossBuyOrders.first() ;
        _stopLossBuyOrders.cancel( buyOrder ) ;
        buyOrder->trigger() ;
        _replyApplication.sendTriggered( buyOrder ) ;

        insertBuy( buyOrder ) ;
      }
    } catch( ListIsEmpty &e ) {}

    try
    {
      while( _marketPictureRecord.getLastTradePrice() <=
             _stopLossSellOrders.first()->getStopPrice() )
      {
        OrderPtr sellOrder = _stopLossSellOrders.first() ;
        _stopLossSellOrders.cancel( sellOrder ) ;
        sellOrder->trigger() ;
        _replyApplication.sendTriggered( sellOrder ) ;

        insertSell( sellOrder ) ;
      }
    } catch( ListIsEmpty &e ) {}
  }

  const MarketPicture::Record &OrderBook::getMarketPictureRecord( )
  {
    MarketData &buyMarketData = _buyOrders.getMarketData( ) ;
    for( int j = 0 ; j < 5 ; j++ )
    {
      _marketPictureRecord.getDepthAt( j ).setBestBuyPrice( buyMarketData.price[j] ) ;
      _marketPictureRecord.getDepthAt( j ).setTotalBuyQty( buyMarketData.qty[j] ) ;
    }

    MarketData &sellMarketData = _sellOrders.getMarketData( );
    for( int j = 0 ; j < 5 ; j++ )
    {
      _marketPictureRecord.getDepthAt( j ).setBestSellPrice( sellMarketData.price[j] ) ;
      _marketPictureRecord.getDepthAt( j ).setTotalSellQty( sellMarketData.qty[j] ) ;
    }

    _hasChanged = false ;
    return _marketPictureRecord ;
  }

  void OrderBook::start()
  {
    _isActive = true ;
  };

  void OrderBook::stop()
  {
    _isActive = false ;

    cancelAll< DescOrderList >( _buyOrders ) ;
    cancelAll< AscOrderList >( _sellOrders ) ;

    cancelAll< AscOrderList >( _stopLossBuyOrders ) ;
    cancelAll< DescOrderList >( _stopLossSellOrders ) ;

    _hasChanged = true ;
  }

}
