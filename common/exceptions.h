#ifndef UT_EXCEPTIONS_H
#define UT_EXCEPTIONS_H

#include <string>
//#include "values.h"

/**
 * Create exceptions that will be used by everyone.
 */
namespace UT
{
  /**
   * General exception class.
   */
  class Exception : public std::exception
  {
    public :
      Exception() {}

      Exception(const std::string errorType, const std::string &what )
               : _type ( errorType), _message ( what )
      {
        _returnMessage = _type ;
        _returnMessage.append( " : " ) ;
        _returnMessage.append( _message ) ;
      }

      ~Exception() throw() {}

      virtual const char* what() const
        throw()
      {
        return _returnMessage.c_str() ;
      }

    private :
      std::string _type;
      std::string _message;
      std::string _returnMessage ;
  };

  /**
   * Exception thrown then there is an error in the configuration.
   */
  class ConfigError : public Exception
  {
    public :
      ConfigError(const std::string &what)
        : Exception("Config Error", what)
      {
      }
  };

  /**
   * Exception thrown when a file does not exist.
   */
  class FileNotFound : public Exception
  {
    public :
      FileNotFound ( const std::string &what ) :
        Exception ( "Could not find file ", what )
    {}
  };

  /**
   * Exception thrown if ecl tries to send an order to an adapter
   *  that has not yet connected.
   */
  class ExchangeNotConnected : public Exception
  {
    public :
      ExchangeNotConnected( )
        : Exception( "Exchange Not Connected ", "" )
      {}
  };

  /**
   * Exception thrown when we are unable to login
   */
  class LoginError : public Exception
  {
    public :
      LoginError ( const std::string &what)
        : Exception("Logon Error", what)
      {
      }
  };

  /**
   * Exception thrown when we are unable to login
   *   However, in this case we will not try to reconnect.
   */
  class LoginErrorEndConnection : public Exception
  {
    public :
      LoginErrorEndConnection ( const std::string &what)
        : Exception("Logon Error", what)
    {
    }

  };

  /**
   * Exception thrown when the password is incorrect.
   */
  class LoginErrorInvalidPassword : public LoginErrorEndConnection
  {
    public :
      LoginErrorInvalidPassword( )
        : LoginErrorEndConnection( "Invalid Password " )
      {}
  };

  /**
   * Exception thrown when the user is already logged on
   */
  class LoginErrorUserAlreadyLoggedOn : public LoginErrorEndConnection
  {
    public :
      LoginErrorUserAlreadyLoggedOn ( )
        : LoginErrorEndConnection( "User is Already Logged On" )
      {}
  };

  /**
   * Exception provided by the exchange.
   */
  class ErrorFromExchange : public Exception
  {
    public :
      ErrorFromExchange ( const std::string &what)
        : Exception("Error From Exchange", what)
      {
      }
  };

  /**
   * If cases where we are given a server port & have to lookup
   *   the actualy server / port from the first set,
   *   this exception will be thrown stating that no more work needs
   *   to be done on this server.
   */
  class LookupComplete : public Exception
  {
    public :
      LookupComplete( )
        : Exception("Lookup complete", "disconnecting")
      {
      }
  };

  /**
   * Exception thrown is an order does not exist.
   */
  class OrderIdDoesNotExist : public Exception
  {
    public :
      OrderIdDoesNotExist( const std::string &orderId )
        : Exception( "Order Id does not exist", orderId )
      {}
  };

  /**
   * Exception thrown when an order does not exist.
   */
  class NumberConvertException : public Exception
  {
    public :
      NumberConvertException ( const std::string &reason )
        : Exception( "Error During Conversion", reason )
      {}
  };

  /**
   * Exception thrown when a module is accessed without first initializing it.
   * This should normally be detected in the development stage and fixed
   * accordingly.
   * Having this exception makes sure that a developer using an api makes sure
   * he has initialized it before calling its functions.
   */
  class NotYetInitializedException : public Exception
  {
    public :
      NotYetInitializedException( const std::string &what )
        : Exception( "Module Has Not Been Initialized", what )
      {
      }
  };

 /**
  * Exception class for the notification module
  * which will handle all the general exceptions
  */
  class NotificationException : public Exception
 {
   public:
     NotificationException (const std::string &what)
       :Exception("Exception occured in notification module ", what)
     {
     }
 };

  /**
   * Exception if we are unable to convert a fix field.
   */
  class FixFieldNotANumberException : public Exception
  {
    public :
      FixFieldNotANumberException( int fieldNum, const std::string &what = "" )
        : Exception( "Field is not a number", what ),
        field( fieldNum )
    {
    }

    public :
      int field ;
  };

}

#endif
