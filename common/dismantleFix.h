#ifndef DISMANTLE_FIX_H
#define DISMANTLE_FIX_H

#ifndef NDEBUG // DO NOT INCLUDE IN RELEASE BUILDS

#include <iostream>
#include <iomanip>

#include <boost/unordered_map.hpp>

namespace UT
{
  /**
   * @brief Accept a Fix String and display it in human readable format.
   */
  class DismantleFix
  {
    public :
      /**
       * @brief Display a fix string in human readable format.
       * This is just a debug message and should not be used in production code.
       * @param Header you wish to add.. example Inbound from OMS, 
       *        outbound to PLUS
       * @param The fixMessage in string format
       * @return  
       */
      static void dismantle( const std::string &direction, 
                             const std::string &fixMessage );

    private :
      /**
       * All of the FIX 4.2 tags we use including some custom tags.
       * The map saves the Key ( Tag Number ) to its description ( Tag Text ).
       */
      static boost::unordered_map< std::string , std::string> _fixTags;

      /**
       * All the Fix Values that we are aware of.
       * The key is created using the tag number and tag value separated by '-'.
       * The value is the description of this tag value.
       */
      static boost::unordered_map< std::string , std::string> _fixTagsDesc;
  };
}

#endif // NDEBUG 

#endif // DISMANTLE_FIX_H
