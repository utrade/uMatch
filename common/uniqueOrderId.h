#ifndef UNIQUE_ORDER_ID_H
#define UNIQUE_ORDER_ID_H

#include <boost/date_time/posix_time/posix_time.hpp>

namespace UT {


/**
 *
 * \class UniqueOrderId
 *
 * Generates a sequential unique ID (number)
 *
 */
  class UniqueOrderId
  {
  public:
    static std::string get();

    static void set(long orderId)
      { _orderId = orderId; }

  private:
    static std::string makeString();
    static long        _orderId;
  };
}

#endif
