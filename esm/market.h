#ifndef ESM_MARKET_H
#define ESM_MARKET_H

#include "orderBook.h"
#include "udpSender.h"
#include "fixMarketDataHandler.h"

namespace ESM
{
  /**
   * @brief The market class maintans all the order books.
   *
   * Order books are maintained on security id.
   * If a new order is placed for a security id that is not associated with
   * the order book, it's order book is created.
   * Cancels & Replaces, on the other hand, are rejected.
   */
  class Market
  {
    public :
      /**
       * @brief Constructor to create a new instance of the market.
       *
       * @param The application whose methods will be called when we wish to
       *          send a confirmation back.
       *
       * @param If market data is enabled, the host to whom the data will be
       *          sent.
       *
       * @param If market data is enabled, the port on which data will be
       *          sent.
       */
      Market( ReplyApplication &replyApplication,
              const std::string &address,
              const std::string &port) ;

      /**
       * @brief Find the order book and insert the order into that order book.
       *        Create the book if it does not already exist.
       *
       * @param The order to be inserted.
       */
      void insert( NewOrderPtr order ) ;

      /**
       * @brief Replace an order on the order book.
       *
       * @param The order which will replace the old order.
       */
      void replace( ReplaceOrderPtr order ) ;

      /**
       * @brief Cancel an order from the order book.
       *
       * @param The order to be cancelled.
       */
      void cancel( CancelOrderPtr order ) ;

      /**
       * @brief Provide a console based ui to the user.
       */
      void readCommands( ) ;

      /**
       * @brief Execute a command from the console based ui.
       *
       * @param The command to be executed.
       */
      bool executeCommand( const std::string &command ) ;

#ifndef UDP_MARKET_DATA
      void setMarketDataApplication(MarketDataApplication* app)
      {
        _mdApplication = app;
      }
#endif

      /**
       * @brief Start accepting orders.
       */
      void start() ;

      /**
       * @brief Stop accepting orders and cancel all orders from the order
       *        books.
       */
      void stop() ;

    private :
      /**
       * The application which will send the confirmations to the client.
       */
      ReplyApplication &_replyApplication ;

      /**
       * The list of order books maintained by this market.
       */
      typedef boost::unordered_map< std::string, OrderBookPtr > OrderBooksMap ;
      OrderBooksMap _orderBooks ;

      /**
       * A market picture a.k.a snapshot which will be sent out periodically.
       */
      MarketPicture _marketPicture ;

      /**
       * Make sure that two threads do not try to create the same order book.
       */
      boost::mutex _mutexForNewBook ;

      /**
       * A pointer to the order books which will be used to create the
       * snapshot.
       */
      std::vector< OrderBookPtr > _orderBooksForMarketPicture ;

      /**
       * @brief Send the market picture to the server on the port.
       */
      void sendMarketPicture() ;

#ifdef UDP_MARKET_DATA
      UdpSender _udpSender ;
#else
      MarketDataApplication* _mdApplication;
#endif
  };
}
#endif // ESM_MARKET_H
