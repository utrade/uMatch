#ifndef ESM_EXCEPTIONS_H
#define ESM_EXCEPTIONS_H

#include <string>
#include "order.h"

namespace ESM
{
  /**
   * The exception classes contain all the exceptions that can occur
   * and need to be handled.
   */

  /**
   * @brief A generic exception class.
   */
  class Exception : public std::exception
  {
    public :
      Exception() {}

      Exception(const std::string errorType, const std::string &what )
               : _type ( errorType), _message ( what )
      { }

      ~Exception() throw() {}

      virtual const char* what() const
        throw()
      {
        std::string msg( _type ) ;
        msg.append( " : " ) ;
        msg.append( _message ) ;

        return msg.c_str() ;
      }

    private :
      std::string _type ;
      std::string _message ;
  };

  /**
   * @brief Exception thrown when a Replace contains incorrect data.
   */
  class RejectReplace : public Exception
  {
    public :
      RejectReplace( const std::string &what )
        : Exception( "Reject Replace ", what )
      {}
  };

  /**
   * @brief Exception thrown when an order contains incorrect data.
   */
  class OrderError : public Exception
  {
    public :
      OrderError( const std::string &what )
        : Exception( "Order Error", what )
      {}
  };

  /**
   * @brief Exception thrown when a container doesn't have any data but we try
   * to access it's element ;
   */
  class ListIsEmpty : public Exception
  {
    public :
      ListIsEmpty( )
        : Exception( "Error ", "List is empty" )
      {}
  };

  /**
   * @brief An order loses priority when it's price is modified.
   * This exception to add handling to push that order to the back of the
   * book, at that price.
   */
  class OrderHasChanged : public Exception
  {
    public :
      OrderHasChanged( )
        : Exception ( "Order Has Changed ", "Lost Priority" )
      {
      }
  };

  /**
   * @brief Exception throw if you try to cancel / replace an order that does
   * not exist or was already filled / cancelled.
   */
  class OrderIdNotFound : public Exception
  {
    public :
      OrderIdNotFound( const std::string &what )
        : Exception( "Cannot find order ", what )
      {}
  };

  /**
   * @brief An exception thrown when an order is placed for a security id
   * which does not exist.
   */
  class SecurityIdNotFound : public Exception
  {
    public :
      SecurityIdNotFound ( const std::string &what )
        : Exception( "Cannot find securityId ", what )
      {}
  };

  /**
   * @brief Only Buy & Sell are supported. This exception is thrown if the
   * side contains any other (FIX) value.
   */
  class SideNotHandled : public Exception
  {
    public :
      SideNotHandled( const std::string &what)
        : Exception( "Side not handled", what )
      {}
  };

  /**
   * @brief Exception thrown when an order contains an OrdType that is not
   * handled.
   */
  class OrderTypeNotHandled : public Exception
  {
    public :
      OrderTypeNotHandled( const std::string &what)
        : Exception( "Order Type Not Handled ", what )
      {}
  };

  /**
   * @brief Exception thrown when an order contains a TimeInForce not handled.
   * handled.
   */
  class TimeInForceNotHandled : public Exception
  {
    public :
      TimeInForceNotHandled( const std::string &what)
        : Exception( "TimeInForce Not Handled ", what )
      {}
  };
}

#endif // ESM_EXCEPTIONS_H

