#include "fixMarketDataHandler.h"
#include <dismantleFix.h>

#include <quickfix/Session.h>
#include <quickfix/fix42/MarketDataSnapshotFullRefresh.h>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

namespace ESM {

  void MarketDataApplication::toApp( FIX::Message&message,
                                     const FIX::SessionID& )
    throw( FIX::DoNotSend )
  {
#ifndef NDEBUG
    UT::DismantleFix::dismantle( "OUTBOUND", message.toString() ) ;
#endif
  }

  void MarketDataApplication::fromApp(const FIX::Message& message,
                                      const FIX::SessionID& sessionId )
    throw( FIX::FieldNotFound,
           FIX::IncorrectDataFormat,
           FIX::IncorrectTagValue,
           FIX::UnsupportedMessageType )

  {
#ifndef NDEBUG
    UT::DismantleFix::dismantle( "INBOUND", message.toString() ) ;
#endif
  }

  void MarketDataApplication::onLogon( const FIX::SessionID& id )
  {
    boost::mutex::scoped_lock lock( _mutexSetSessions );
    _setSessions.insert( id );
  }

  void MarketDataApplication::onLogout( const FIX::SessionID& id )
  {
    boost::mutex::scoped_lock lock( _mutexSetSessions );
    _setSessions.erase( id );
  }

  void MarketDataApplication::send( const MarketPicture& message )
  {
    for (int i = 0; i < message.getNoOfRecs(); i++ )
    {
      const MarketPicture::Record& mpRecord = message.getRecordAt( i );

      FIX42::MarketDataSnapshotFullRefresh mdSnapshot;
      mdSnapshot.set(
        FIX::SecurityID(
          boost::lexical_cast<std::string>( mpRecord.getScripCode() )
          )
        );
      FIX42::MarketDataSnapshotFullRefresh::NoMDEntries group;

      for (int j = 0; j < 5; j++)
      {
        long buyPrice = mpRecord.getDepthAt( i ).getBestBuyPrice();
        long buyQty = mpRecord.getDepthAt( i ).getTotalBuyQty();
        long sellPrice = mpRecord.getDepthAt( i ).getBestSellPrice();
        long sellQty = mpRecord.getDepthAt( i ).getTotalSellQty();

        bool buyNotAvail = false;
        bool sellNotAvail = false;

        if ( buyPrice != 0 && buyQty != 0 )
        {
          group.set( FIX::MDEntryType( FIX::MDEntryType_BID ) );
          group.set( FIX::MDEntryPx( buyPrice ) );
          group.set( FIX::MDEntrySize( buyQty ) );
          mdSnapshot.addGroup( group );
        }
        else
          buyNotAvail = true;

        if ( sellPrice != 0 && sellQty != 0 )
        {
          group.set( FIX::MDEntryType( FIX::MDEntryType_OFFER ) );
          group.set( FIX::MDEntryPx( sellPrice ) );
          group.set( FIX::MDEntrySize( sellQty ) );
          mdSnapshot.addGroup( group );
        }
        else
          sellNotAvail = true;

        if ( buyNotAvail && sellNotAvail )
          break;
      }

      try {
        boost::mutex::scoped_lock lock( _mutexSetSessions );
        for ( std::set<FIX::SessionID>::iterator it = _setSessions.begin();
              it != _setSessions.end();
              it++ )
        {
          FIX::SessionID session = *it;
          FIX::Session::sendToTarget ( mdSnapshot, session ) ;
        }
      }
      catch (boost::lock_error& e)
      {
        std::cout << "lock error: " << e.what() << std::endl;
      }
    }
  }

}
