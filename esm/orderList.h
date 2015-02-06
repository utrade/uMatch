#ifndef ESM_ORDER_LISTH_H
#define ESM_ORDER_LISTH_H

#include <map>
#include <boost/unordered_map.hpp>

#include "../common/definesForCreateEndianless.h"
#include "order.h"

namespace ESM
{
  struct MarketData
  {
      UT::LONG price[5] ;
      UT::LONG qty[5] ;
  };

  /**
   *
   * \class OrderList
   *
   * OrderList to store the buy and sell orders
   *
   */
  template< class Compare >
  class OrderList
  {
    typedef std::multimap < long, OrderPtr, Compare > OrdersByPriceMap;
    typedef boost::unordered_map< std::string,
            typename OrdersByPriceMap::iterator > OrdersByOrderIdMap ;

    public :

    /**
     * @brief Get the first order from the list which will be used to match
     * against a new order.
     *
     * @return First order in the list.
     */
    OrderPtr first()
    {
      _iOrdersByPrice = _ordersByPrice.begin() ;
      if( _iOrdersByPrice == _ordersByPrice.end() )
      {
        throw ListIsEmpty() ;
      }
      return _iOrdersByPrice->second ;
    }

    /**
     * @brief Insert a new order into this list.
     *
     * @param The price at which the order should be inserted.
     *
     * @param The order to be inserted.
     *
     * @return True if successfully inserted.
     */
    bool insert( long price, OrderPtr order )
    {
      _iOrdersByPrice = _ordersByPrice.insert (
          typename OrdersByPriceMap::value_type ( price, order )
          ) ;
      _ordersByOrderId.insert (
          std::make_pair ( order->getOrderId(),  _iOrdersByPrice )
          ) ;
      return true ;
    }

    /**
     * @brief Cancel an order from the list.
     *
     * @param The order to be cancelled.
     *
     * @return The order that was cancelled.
     */
    OrderPtr cancel( OrderPtr order )
    {
      _iOrdersByOrderId = _ordersByOrderId.find( order->getOrderId() ) ;
      if( _iOrdersByOrderId == _ordersByOrderId.end() )
      {
        throw OrderIdNotFound( order->getOrderId() ) ;
      }
      OrderPtr oldOrder = _iOrdersByOrderId->second->second ;
      oldOrder->cancel( *order ) ;

      _ordersByPrice.erase( _iOrdersByOrderId->second ) ;
      _ordersByOrderId.erase( _iOrdersByOrderId ) ;

      return oldOrder ;
    }

    /**
     * @brief Replace an order in the list.
     *
     * @param The order that will replace the old order, identified by the
     * orderid.
     *
     * @return the old order.
     */
    OrderPtr replace( OrderPtr order )
    {
      _iOrdersByOrderId = _ordersByOrderId.find( order->getOrderId() ) ;
      if( _iOrdersByOrderId == _ordersByOrderId.end() )
      {
        throw OrderIdNotFound( order->getOrderId() ) ;
      }

      OrderPtr oldOrder = _iOrdersByOrderId->second->second ;

      oldOrder->replace( *order ) ;

      return oldOrder ;
    }

    /**
     * @brief Get the top 5 prices & quantities in this list.
     *
     * @return The top of this list.
     */
    MarketData &getMarketData( )
    {
      _marketData.price[0] = 0 ;
      _marketData.qty[0] = 0 ;
      _marketData.price[1] = 0 ;
      _marketData.qty[1] = 0 ;
      _marketData.price[2] = 0 ;
      _marketData.qty[2] = 0 ;
      _marketData.price[3] = 0 ;
      _marketData.qty[3] = 0 ;
      _marketData.price[4] = 0 ;
      _marketData.qty[4] = 0 ;

      _counter = 0 ;
      _iOrdersByPriceForMarketData = _ordersByPrice.begin() ;

      if ( _iOrdersByPriceForMarketData != _ordersByPrice.end() )
      {
        _marketData.price[_counter] = _iOrdersByPriceForMarketData->first ;
        _marketData.qty[_counter] = _iOrdersByPriceForMarketData->second->getPendingQty() ;

        while ( ++_iOrdersByPriceForMarketData != _ordersByPrice.end() )
        {
          if( _marketData.price[_counter] == _iOrdersByPriceForMarketData->first )
          {
            _marketData.qty[_counter] += _iOrdersByPriceForMarketData->second->getPendingQty() ;
          }
          else
          {
            if( ++_counter == 5 ) break ;

            _marketData.price[_counter] = _iOrdersByPriceForMarketData->first ;
            _marketData.qty[_counter] = _iOrdersByPriceForMarketData->second->getPendingQty() ;
          }
        }
      }
      else
      {
        //std::cout << "i didnt find any market data" << std::endl ;
      }

      return _marketData ;
    }

    /**
     * @brief Print this list.
     */
    void print()
    {
      std::cout << "=============BEGIN============================" << std::endl ;
      for( _iOrdersByPrice = _ordersByPrice.begin() ;
          _iOrdersByPrice != _ordersByPrice.end() ;
          ++_iOrdersByPrice )
      {
        _iOrdersByPrice->second->print() ;
      }
      std::cout << "=============END============================\n\n" << std::endl ;

    }

    /**
     * @brief Remove an order from the map.
     */
    OrderPtr erase( const OrderPtr &order )
    {
      return erase( order->getOrderId() ) ;
    }

    /**
     * @brief Remove an order from the map.
     */
    OrderPtr erase( const std::string &orderId )
    {
      _iOrdersByOrderId = _ordersByOrderId.find( orderId ) ;

      if( _iOrdersByOrderId != _ordersByOrderId.end() )
      {
        OrderPtr order = _iOrdersByOrderId->second->second ;

        _ordersByPrice.erase( _iOrdersByOrderId->second ) ;
        _ordersByOrderId.erase( _iOrdersByOrderId ) ;
        return order ;
      }
      throw OrderIdNotFound( orderId ) ;
    }

    /**
     * @brief Fill an order and erase it if it's filled.
     */
    void fill( long price, long qty )
    {
      OrderPtr order = first() ;
      order->fill( price, qty ) ;
      if( order->getPendingQty() == 0)
      {
        erase( order ) ;
      }
    }

    private :
    /**
     * A map which maintans the price & time priority.
     */
    OrdersByPriceMap _ordersByPrice ;

    /**
     * A map which maintains the orderis and the order.
     * Used for replace & cancel.
     */
    OrdersByOrderIdMap _ordersByOrderId ;

    /**
     * The snapshot in memory.
     */
    MarketData _marketData ;

    typename OrdersByPriceMap::iterator _iOrdersByPrice;
    typename OrdersByOrderIdMap::iterator _iOrdersByOrderId;

    typename OrdersByPriceMap::iterator _iOrdersByPriceForMarketData;
    int _counter ;
  };

  /* Order book sorted by price Ascending */
  class AscOrderList : public OrderList < std::less<long> > {};

  /* Order book sorted by price Descending */
  class DescOrderList : public OrderList < std::greater< long> > { };
}

#endif // ESM_ORDER_LISTH_H
