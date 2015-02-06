
#include "market.h"

#include <boost/algorithm/string.hpp>
#include <boost/thread.hpp>

namespace ESM
{
  Market::Market( ReplyApplication &replyApplication,
                  const std::string &address,
                  const std::string &port)
#ifdef UDP_MARKET_DATA
    : _replyApplication( replyApplication ) ,
      _udpSender( address, port )
#else
    : _replyApplication( replyApplication )
#endif
  {
#ifdef UDP_MARKET_DATA
    std::cout << "Sending market data on  : " << address << ":" << port << std::endl ;
#endif
    boost::thread marketPictureThread( &Market::sendMarketPicture, this ) ;
  }

  void Market::insert( NewOrderPtr order )
  {
    OrderBooksMap::iterator iOrderBooks = _orderBooks.find( order->getSecurityId() ) ;

    if( iOrderBooks == _orderBooks.end() )
    {
      boost::mutex::scoped_lock lock( _mutexForNewBook ) ;
      iOrderBooks = _orderBooks.find( order->getSecurityId() ) ;
      if( iOrderBooks == _orderBooks.end() )
      {
        OrderBookPtr newOrderBook =
          OrderBookPtr( new OrderBook( _replyApplication, order ) ) ;

        iOrderBooks = _orderBooks.insert(
          std::make_pair( order->getSecurityId(), newOrderBook )
          ).first ;
        _orderBooksForMarketPicture.push_back( newOrderBook ) ;
      }
    }

    iOrderBooks->second->insert( order ) ;
  }

  void Market::replace( ReplaceOrderPtr order )
  {
    OrderBooksMap::iterator iOrderBooks = _orderBooks.find( order->getSecurityId() ) ;
    if( iOrderBooks == _orderBooks.end() )
    {
      throw SecurityIdNotFound( order->getSecurityId() ) ;
    }
    iOrderBooks->second->replace( order ) ;
  }

  void Market::cancel( CancelOrderPtr order )
  {
    OrderBooksMap::iterator iOrderBooks = _orderBooks.find( order->getSecurityId() ) ;
    if( iOrderBooks == _orderBooks.end() )
    {
      throw SecurityIdNotFound( order->getSecurityId() ) ;
    }
    iOrderBooks->second->cancel( order ) ;
  }

  void Market::readCommands( )
  {
    std::string command = "";
    while( command != "quit" )
    {
      std::getline ( std::cin, command ) ;
      executeCommand( command ) ;
    }
  }

  bool Market::executeCommand( const std::string &command )
  {
    if( command == "stop" || command == "quit" || command == "q" ) {
      stop() ;
    }
    if( command == "start" ) {
      start() ;
    }
    else
    {
      std::cout << "The commands you can use are : \n"
                << " q/quit : Quit the application. This will cancel all pending orders \n"
                << " stop   : Cancel pending orders and do not accept new orders \n"
                << " start  : Begin accepting new orders. Used after stop \n"
                << std::endl ;
    }

    return false ;
  }

  void Market::stop()
  {
    OrderBooksMap::iterator iOrderBooks = _orderBooks.begin() ;

    while( iOrderBooks != _orderBooks.end() )
    {
      std::cout << "Stopping Order Book of " << iOrderBooks->first << "  " ;
      iOrderBooks->second->stop() ;
      ++iOrderBooks ;
      std::cout << std::endl ;
    }
  }

  void Market::start()
  {
    OrderBooksMap::iterator iOrderBooks = _orderBooks.begin() ;

    while( iOrderBooks != _orderBooks.end() )
    {
      std::cout << "Starting Order Book of " << iOrderBooks->first << std::endl;
      iOrderBooks->second->start() ;
      ++iOrderBooks ;
    }
  }

  void Market::sendMarketPicture()
  {
    int i ;
    while( true )
    {
      for( i = _orderBooksForMarketPicture.size() - 1; i >= 0 ; i -- )
      {
        if( _orderBooksForMarketPicture[i]->hasChanged() )
        {
          _marketPicture.addRecord(
            _orderBooksForMarketPicture[i]->getMarketPictureRecord()
            ) ;

          if( _marketPicture.getNoOfRecs() == MarketPicture::MaxNoOfRecs )
          {
#ifdef UDP_MARKET_DATA
            _udpSender.send( _marketPicture ) ;
#else
            _mdApplication->send( _marketPicture );
#endif
            _marketPicture.reset() ;
          }
        }
      }

      if( _marketPicture.getNoOfRecs() > 0 )
      {
#ifdef UDP_MARKET_DATA
            _udpSender.send( _marketPicture ) ;
#else
            _mdApplication->send( _marketPicture );
#endif
        _marketPicture.reset() ;
      }

      boost::this_thread::sleep(boost::posix_time::seconds(1));
    }
  }
}
