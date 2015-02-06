#ifndef ESM_REPLY_APPLICATION_H
#define ESM_REPLY_APPLICATION_H

#include <boost/thread.hpp>
#include "order.h"

namespace ESM {

  /**
   * \class ReplyApplication
   *
   * This class is used to generate and send the execution reports back to the
   * FIX client
   *
   */
  class ReplyApplication
  {
    public :
      /**
       * @brief Send a new order confirmation to the client.
       *
       * @param The order we will use to generate the confirmation.
       */
      void sendNewConfirm( OrderPtr order ) ;

      /**
       * @brief Send a replace order confirmation to the client.
       *
       * @param The order we will use to generate the confirmation.
       */
      void sendReplaceConfirm( OrderPtr order ) ;

      /**
       * @brief Send a cancel order confirmation to the client.
       *
       * @param The order we will use to generate the confirmation.
       */
      void sendCancelConfirm( OrderPtr order, const std::string &reason ) ;

      /**
       * @brief Send a New Order Reject to the client.
       *
       * @param The order we will use to generate the reject.
       */
      void sendNewReject( OrderPtr order, const std::string &reason );

      /**
       * @brief Send a Replace Order Reject to the client.
       *
       * @param The order we will use to generate the reject.
       */
      void sendReplaceReject( OrderPtr order, const std::string &reason );

      /**
       * @brief Send a Cancel Order Reject to the client.
       *
       * @param The order we will use to generate the reject.
       */
      void sendCancelReject( OrderPtr order, const std::string &reason );

      /**
       * @brief A market order that could not be filled was converted to
       * limit. Send the confirmation to the client.
       *
       * @param The order we will use to generate the confirmation.
       */
      void sendMarketToLimit( OrderPtr order ) ;

      /**
       * @brief Send a stop loss trigger confirmation to the client.
       *
       * @param The order we will use to generate the confirmation.
       */
      void sendTriggered( OrderPtr order ) ;

      /**
       * @brief Send trade confirmation to the client.
       *
       * @param The order we will use to send the trade confirmation.
       */
      void sendFillConfirm( OrderPtr order ) ;
  };
}
#endif // ESM_REPLY_APPLICATION_H
