#ifndef ESM_ORDER_BOOK_H
#define ESM_ORDER_BOOK_H

#include <boost/thread.hpp>

#include "orderList.h"
#include "replyApplication.h"

namespace ESM
{
  /**
   *
   * \class OrderBook
   *
   * OrderBook stores the orders received for a particular instrument
   * and does the matching. Additionally, it prepares the market data
   * snapshot which is to be send out.
   *
   */
  class OrderBook
  {
    public :
      /**
       * @brief The constructor accepts the reply application which will be
       * used to send responses and the first order which will be used to
       * calculate the open, close, upper & lower limits.
       *
       * @param The reply application which will send confirmations.
       *
       * @param The first order used to caculate open, close & limits.
       */
      OrderBook( ReplyApplication &replyApplication, OrderPtr order ) ;

      /**
       * @brief Insert a new order into the order book.
       *
       * @param The order to be inserted.
       */
      void insert( OrderPtr order ) ;

      /**
       * @brief Replace an order in the order book.
       *
       * @param The new order that will replace the old one.
       */
      void replace( OrderPtr order ) ;

      /**
       * @brief Cancel and order from the order book.
       *
       * @param The order to be cancelled. Just need the order id.
       */
      void cancel( OrderPtr order ) ;

      /**
       * @brief Convert the buy & sell books into a snapshot and return it.
       *
       * @return A market picture of the current buy & sell books.
       */
      const MarketPicture::Record &getMarketPictureRecord( ) ;

      /**
       * @brief Has the snapshot changed since the last getMarketPictureRecord ?
       *
       * @return True if snapshot has changed.
       */
      bool hasChanged() { return _hasChanged ; }

      /**
       * @brief Start accepting orders.
       */
      void start() ;

      /**
       * @brief Stop accepting orders and cancel orders in order book.
       */
      void stop() ;

    private :
      /**
       * The buy order book.
       */
      DescOrderList _buyOrders ;

      /**
       * The sell order book.
       */
      AscOrderList _sellOrders ;

      /**
       * The stop loss buy order book.
       */
      AscOrderList _stopLossBuyOrders ;

      /**
       * The stop loss sell order book.
       */
      DescOrderList _stopLossSellOrders ;

      /**
       * The application which will send the replies to the client
       */
      ReplyApplication &_replyApplication ;

      /**
       * Variable to notify if the order book has changed since the last
       * getMarketData request.
       */
      bool _hasChanged ;

      /**
       * Current snapshot of this order book.
       */
      MarketPicture::Record _marketPictureRecord ;

      /**
       * A mutex to make sure only one transaction occurs on this order book
       * at a time.
       */
      boost::mutex _mutexOnMatch ;

      /**
       * set to false when we stop() this.
       */
      bool _isActive ;

      /**
       * @brief Try to match a buy order. 
       *        If corresponding sell is unavailable, insert it into the buy
       *        order book.
       *
       * @param The order to be inserted.
       */
      void insertBuy( OrderPtr order ) ;

      /**
       * @brief Try to match a sell order. 
       *        If corresponding buy is unavailable, insert it into the sell
       *        order book.
       *
       * @param The order to be inserted.
       */
      void insertSell( OrderPtr order ) ;

      /**
       * @brief Insert a stop loss buy order into its book and check whether
       * it has to be triggered.
       *
       * @param The order to be inserted.
       */
      void insertStopLossBuy( OrderPtr order ) ;

      /**
       * @brief Insert a stop loss sell order into its book and check whether
       * it has to be triggered.
       *
       * @param The order to be inserted.
       */
      void insertStopLossSell( OrderPtr order ) ;

      /**
       * @brief Replace a buy with a new order.
       *
       * @param The order that will replace the old one.
       */
      void replaceBuy( OrderPtr order ) ;

      /**
       * @brief Replace a sell with a new order.
       *
       * @param The order that will replace the old one.
       */
      void replaceSell( OrderPtr order ) ;

      /**
       * @brief After a successful trade, check if any order needs to be
       * triggered from stop to limit / market based on it's stop price.
       */
      void checkTriggeredOrders( ) ;

      /**
       * @brief Update the market data.
       */
      void updateMarketData( int price, int qty ) ;

      void print() ;

      /**
       * @brief Cancel all the orders from the list.
       *        Can be buy, sell, stopBuy or stopSell.
       *
       * @param The order list ot be cancelled.
       */
      template< class CancelOrderList >
        void cancelAll( CancelOrderList &list )
        {
          char displayChar = '\\' ;

          OrderPtr canceledOrder ;

          try
          {
            // When the list is over, we will get a exception ListIsEmpty thrown
            while( true )
            {
              canceledOrder = list.first() ;
              canceledOrder = list.cancel( canceledOrder ) ;
              _replyApplication.sendCancelConfirm(
                canceledOrder,
                "Order Cancelled As System Is Shutting Down"
                ) ;

              std::cout << '\b' << displayChar  ;
              switch( displayChar )
              {
                case '\\' :
                  displayChar = '|' ;
                  break ;
                case '|' :
                  displayChar = '/' ;
                  break ;
                case '/' :
                  displayChar = '-' ;
                  break ;
                case '-' :
                  displayChar = '\\' ;
                  break ;
              }

              _hasChanged = true ;
            }
            std::cout << '\b' ;
          }
          catch( ListIsEmpty &e) {
          }
        }
  };

  typedef boost::shared_ptr< OrderBook > OrderBookPtr ;
}
#endif // ESM_ORDER_BOOK_H
