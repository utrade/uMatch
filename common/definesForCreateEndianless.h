#ifndef UT_DEFINES_FOR_CREATE_ENDIANLESS_H
#define UT_DEFINES_FOR_CREATE_ENDIANLESS_H

//#include <endian.h>
#include "convertor.h"
#include "types.h"
#include "definesForString.h"
// #include "constants.h"

/****************************************************
*  Defines for creating field                       *
****************************************************/
#define UT_CREATE_FIELD_SIGNED( NAME, TYPE )                                   \
  public : UT::TYPE get##NAME() const {  return _##NAME ; }                    \
  public : UT::TYPE &getRef##NAME() {  return _##NAME ; }                      \
  public : void set##NAME( UT::TYPE value ) { _##NAME = value ; }              \
  public : void set##NAME##FromString( const std::string &value ) {            \
             set##NAME( UT::IntConvertor::convert( value ) ) ; }               \
  private : UT::TYPE _##NAME

#define UT_CREATE_FIELD_UNSIGNED( NAME, TYPE )                                 \
  public : UT::TYPE get##NAME() const {  return _##NAME ; }                    \
  public : UT::TYPE &getRef##NAME() {  return _##NAME ; }                      \
  public : void set##NAME( UT::TYPE value ) { _##NAME = value ; }              \
  public : void set##NAME##FromString( const std::string &value ) {            \
             set##NAME( UT::UnsignedIntConvertor::convert( value ) ) ; }       \
  private : UT::TYPE _##NAME

#define UT_CREATE_INT8( NAME )                                                 \
  UT_CREATE_FIELD_SIGNED( NAME, INT8 )

#define UT_CREATE_UINT8( NAME )                                                \
  UT_CREATE_FIELD_UNSIGNED( NAME, INT8 )

#define UT_CREATE_SHORT( NAME )                                                \
  UT_CREATE_FIELD_SIGNED( NAME, SHORT )

#define UT_CREATE_USHORT( NAME )                                               \
  UT_CREATE_FIELD_UNSIGNED( NAME, USHORT )

#define UT_CREATE_INT( NAME )                                                  \
  UT_CREATE_FIELD_SIGNED( NAME, INT )

#define UT_CREATE_UINT( NAME )                                                 \
  UT_CREATE_FIELD_UNSIGNED( NAME, UINT )

#define UT_CREATE_LONG( NAME )                                                 \
  UT_CREATE_FIELD_SIGNED( NAME, LONG )

#define UT_CREATE_ULONG( NAME )                                                \
  UT_CREATE_FIELD_UNSIGNED( NAME, ULONG )

#define UT_CREATE_LONGLONG( NAME )                                             \
  UT_CREATE_FIELD_SIGNED( NAME, LONGLONG )

#define UT_CREATE_ULONGLONG( NAME )                                            \
  UT_CREATE_FIELD_UNSIGNED( NAME, ULONGLONG )

#define UT_CREATE_TIME_IN_NANOSECONDS(NAME)                                    \
  UT_CREATE_ULONGLONG(NAME) ;                                                  \
  public : UT::ULONGLONG get##NAME##AsUtc() const {                            \
             return ( _##NAME / SECONDS_TO_NANOSECONDS ) ; }                   \
  public : UT::ULONGLONG get##NAME##AsLocal() const {                          \
             return ( ( _##NAME / SECONDS_TO_NANOSECONDS )                     \
                      + TIMEZONE_OFFSET_UTC_TO_IST ) ; }

#define UT_CREATE_DOUBLE( NAME )                                               \
  public : double get##NAME() const { return _##NAME.value ; }                 \
  public : void set##NAME( double value ) { _##NAME.value = value ; }          \
  public : void set##NAME##FromString( const std::string &value ) {            \
             _##NAME.value = UT::DoubleConvertor::convert( value ) ; }         \
  private: DOUBLE _##NAME

/*
#define UT_CREATE_PRICE( NAME )                                                \
  UT_CREATE_FIELD( NAME, PRICE )
  */
#define UT_CREATE_PRICE( NAME )                                               \
  UT_CREATE_DOUBLE( NAME )

#endif
