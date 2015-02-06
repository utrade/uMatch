
#ifndef ESM_STRUCTURES_H
#define ESM_STRUCTURES_H

#include "../common/definesForCreateEndianless.h"
#include "../common/errorlog.h"

#define UT_CREATE_RECORD( OBJECT ) \
  private : Record _Records[ MaxNoOfRecs ] ; \
  public : \
  const Record &getRecordAt( int position ) const \
  { return _Records[position] ; } \
  Record &getRecordAt( int position ) \
  { return _Records[position] ; } \
  void addRecord( const Record &record ) \
  {  memcpy( &_Records[_NoOfRecs], &record, sizeof( Record ) ) ; \
    _NoOfRecs++; \
    setMsgLen( getMsgLen() + sizeof( Record ) ) ; \
  } \
  void reset() { \
    _NoOfRecs = 0 ; \
    setMsgLen( sizeof( OBJECT ) - MaxNoOfRecs * sizeof( Record ) ); }

namespace ESM
{
  enum Side
  {
    Side_BUY = 1,
    Side_SELL = 2,
    Side_SELL_SHORT = 5
  };

  enum OrderType
  {
    OrderType_MARKET = 1,
    OrderType_LIMIT = 2,
    OrderType_STOP = 3,
    OrderType_STOP_LIMIT = 4
  };

  enum TimeInForce
  {
    TimeInForce_DAY,
    TimeInForce_IOC
  };

  struct Header
  {
    UT_CREATE_LONG( SlotNo ) ;
    UT_CREATE_LONG( MsgLen ) ;
    UT_CREATE_LONG( MsgType ) ;

    public :
    Header( const UT::LONG &msgLen,
            const UT::LONG &msgType )
      : _SlotNo( 0 ),
        _MsgLen( msgLen - 8 ),
        _MsgType( msgType )
    {}

    void print() const
    {
      DEBUG_2( "SlotNo :  ", _SlotNo );
      DEBUG_2( "MsgLen :  ", _MsgLen );
      DEBUG_2( "MsgType :  ", _MsgType );
    }
  };
  typedef Header Message ;

  const UT::LONG MsgType_MARKET_PICTURE = 1906 ;
  struct MarketPicture : public Header //1906
  {
    enum MAX { MaxNoOfRecs = 10 } ;

    struct Record
    {
      struct Depth
      {
        UT_CREATE_LONG( BestBuyPrice ) ;
        UT_CREATE_LONG( TotalBuyQty ) ;
        UT_CREATE_LONG( BestSellPrice ) ;
        UT_CREATE_LONG( TotalSellQty ) ;

        public :
        Depth()
          : _BestBuyPrice( 0 ),
            _TotalBuyQty( 0 ),
            _BestSellPrice( 0 ),
            _TotalSellQty( 0 )
        {}

        void print() const
        {
          std::cout << _TotalBuyQty << " - " << _BestBuyPrice << " | "
                    << _BestSellPrice << " - " << _TotalSellQty
                    << std::endl;
        }
      };

      UT_CREATE_LONG( ScripCode ) ;
      UT_CREATE_LONG( OpenPrice ) ;
      UT_CREATE_LONG( ClosePrice ) ;
      UT_CREATE_LONG( HighPrice ) ;
      UT_CREATE_LONG( LowPrice ) ;
      UT_CREATE_LONG( NoOfTrades ) ;
      UT_CREATE_LONG( Volume ) ;
      UT_CREATE_LONG( Value ) ;
      UT_CREATE_LONG( LastTradeQty ) ;
      UT_CREATE_LONG( LastTradePrice ) ;
      UT_CREATE_LONG( TotalBuyQty ) ;
      UT_CREATE_LONG( TotalSellQty ) ;
      UT_CREATE_CHAR( TradeValueFlag ) ;
      UT_CREATE_CHAR( Trend ) ;
      UT_CREATE_CHAR( SixLakhFlag ) ;
      UT_CREATE_CHAR( AllNoneFlag ) ;
      UT_CREATE_LONG( LowerCktLimit ) ;
      UT_CREATE_LONG( UpperCktLimit ) ;
      UT_CREATE_LONG( WeightedAvgPrice ) ;
      UT_INCLUDE_STRUCT_ARRAY( Depth, 5 ) ;

      public :
      Record()
        : _ScripCode( 0 ), _OpenPrice( -1 ), _ClosePrice( -1 ), _HighPrice( -1 ),
        _LowPrice( -1 ), _NoOfTrades( 0 ), _Volume( 0 ), _Value( 0 ),
        _LastTradeQty( 0 ), _LastTradePrice( 0 ), _TotalBuyQty( 0 ), _TotalSellQty( 0 ),
        _TradeValueFlag( 'N' ), _Trend( '+' ), _SixLakhFlag( 'N' ), _AllNoneFlag( 'N' ),
        _LowerCktLimit( 0 ), _UpperCktLimit( 0 ), _WeightedAvgPrice( 0 )
      {}

      void print() const
      {
        DEBUG_1( "MsgType Is MarketPictureDetail ") ;

        DEBUG_2( "ScripCode :  ", _ScripCode );
        DEBUG_2( "OpenPrice :  ", _OpenPrice );
        DEBUG_2( "ClosePricPrice ", _ClosePrice );
        DEBUG_2( "HighPrice :  ", _HighPrice );
        DEBUG_2( "LowPrice :  ", _LowPrice );
        DEBUG_2( "NoOfTrades :  ", _NoOfTrades );
        DEBUG_2( "Volume :  ", _Volume );
        DEBUG_2( "Value :  ", _Value );
        DEBUG_2( "LastTradeQty :  ", _LastTradeQty );
        DEBUG_2( "LastTradePrice :  ", _LastTradePrice );
        DEBUG_2( "TotalBuyQty :  ", _TotalBuyQty );
        DEBUG_2( "TotalSellQty :  ", _TotalSellQty );
        DEBUG_2( "TradeValueFlag :  ", _TradeValueFlag );
        DEBUG_2( "Trend :  ", _Trend );
        DEBUG_2( "SixLakhFlag :  ", _SixLakhFlag );
        DEBUG_2( "AllNoneFlag :  ", _AllNoneFlag );
        DEBUG_2( "LowerCktLimit :  ", _LowerCktLimit );
        DEBUG_2( "UpperCktLimit :  ", _UpperCktLimit );
        DEBUG_2( "WeightedAvgPrice :  ", _WeightedAvgPrice );
        for( int i = 0 ; i < 5 ; i ++ )
        {
          _Depth[i].print() ;
        }
      }
    };

    UT_CREATE_SHORT( SequenceNo ) ;
    UT_CREATE_SHORT( TradingSession ) ;
    UT_CREATE_SHORT( NoOfRecs ) ;
    UT_CREATE_SHORT( Filler ) ;
    UT_CREATE_RECORD( MarketPicture ) ;

    public :
    MarketPicture()
      : Header( sizeof( MarketPicture ) , MsgType_MARKET_PICTURE ),
        _SequenceNo( 0 ), _TradingSession( 3 ),
        _NoOfRecs( 0 ), _Filler( 0 )
    {
      reset();
    }

    void print() const
    {
      DEBUG_1( "MsgType Is MarketPictureHeader ") ;

      DEBUG_2( "SequenceNo :  ", _SequenceNo );
      DEBUG_2( "TradingSession :  ", _TradingSession );
      DEBUG_2( "NoOfRecs :  ", _NoOfRecs );
      DEBUG_2( "Filler :  ", _Filler );
      for( int i = 0 ; i < _NoOfRecs ; i ++ )
      {
        _Records[i].print() ;
      }
    }
  };

}

#endif // ESM_STRUCTURES_H
