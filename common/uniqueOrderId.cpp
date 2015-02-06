#include "uniqueOrderId.h"
#include <sstream>

namespace UT {

  long UniqueOrderId::_orderId;

  std::string UniqueOrderId::get()
  {
    if ( _orderId == 0 )
    {
      boost::posix_time::ptime currTime =
        boost::posix_time::microsec_clock::local_time();
      boost::posix_time::ptime epoch =
        boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1));
      _orderId = (currTime - epoch).total_milliseconds();
    }
    else
      ++_orderId;

    return makeString();
  }

  std::string UniqueOrderId::makeString()
  {
    std::stringstream ss;
    ss << std::setw(10)
       << std::setfill('0')
       << _orderId;
    return ss.str();
  }
}
