
#ifndef UT_DEFINES_FOR_STRING_H
#define UT_DEFINES_FOR_STRING_H

/****************************************************
*  Fields Not affected by endianess                 *
****************************************************/

#define UT_CREATE_BIT( NAME ) \
  public : \
  bool is##NAME() const { return _##NAME ; }  \
  void is##NAME( bool value ) { _##NAME = value ; } \
  bool get##NAME() const { return _##NAME ; }  \
  private : unsigned char _##NAME : 1 

#define UT_CREATE_MULTI_BIT( NAME, SIZE ) \
  public : \
  bool get##NAME() const { return _##NAME ; }  \
  void set##NAME( bool value ) { _##NAME = value ; } \
  private : unsigned _##NAME : SIZE

#define UT_CREATE_CHAR( NAME ) \
  public : \
  UT::CHAR get##NAME() const { return _##NAME ; }  \
  void set##NAME( UT::CHAR value ) { _##NAME = value ; } \
  private : UT::CHAR _##NAME 

#define UT_CREATE_UCHAR( NAME ) \
  public : \
  UCHAR get##NAME() const { return _##NAME ; }  \
  void set##NAME( UCHAR value ) { _##NAME = value ; } \
  private : UCHAR _##NAME 

/****************************************************
*  Character and string fields                      *
****************************************************/
#define UT_CREATE_FIELD_STRING( NAME, SIZE ) \
  private : char _##NAME[SIZE] 

#define UT_CREATE_STRING( NAME, SIZE ) \
  public : const char *get##NAME() const { return _##NAME ; } \
  public : char *getRef##NAME() { return _##NAME ; } \
  public : void set##NAME( const std::string &value ) \
    { value.copy( _##NAME, value.size() ) ; \
      _##NAME[value.size()] = '\0' ; } \
  UT_CREATE_FIELD_STRING( NAME, SIZE )

#define UT_CREATE_PADDED_STRING( NAME, SIZE ) \
  public : std::string get##NAME() const { return std::string( _##NAME, 0, SIZE ) ; } \
  public :void set##NAME( const std::string &value ) \
    { value.copy( _##NAME, value.size() ) ; } \
  private : char _##NAME[SIZE] 

#define UT_CREATE_PADDED_TERMINATED_STRING( NAME, SIZE ) \
  public : std::string get##NAME() const { return std::string( _##NAME, SIZE ) ; } \
  public :void set##NAME( const std::string &value ) \
    { if( value.size() <= SIZE ) { \
         value.copy( _##NAME, value.size() ) ; \
         memset( ( char *) _##NAME + value.size(), ' ', SIZE - value.size() ) ;\
      }\
      else { value.copy( _##NAME, SIZE ) ; }\
      _##NAME[ SIZE - 1 ] = '\0' ; } \
  UT_CREATE_FIELD_STRING( NAME, SIZE )

/****************************************************
*  Include Structures                               *
****************************************************/

#define UT_INCLUDE_STRUCT( STRUCT, NAME ) \
  public : \
  const STRUCT &get##NAME() const { return _##NAME ; } \
  STRUCT &get##NAME() { return _##NAME ; } \
  void set##NAME( const STRUCT &l##NAME ) { memcpy( &_##NAME, &l##NAME, sizeof( STRUCT )) ; } \
  private : STRUCT _##NAME 

#define UT_INCLUDE_STRUCT_ARRAY( NAME, SIZE ) \
  public : \
  const NAME &get##NAME##At( int pos ) const { \
    return _##NAME[pos] ; }\
  NAME &get##NAME##At( int pos ) { \
    return _##NAME[pos] ; }\
  private : NAME _##NAME[ SIZE ] 

//#if defined(__WIN32__) || defined(_MSC_VER)
#ifdef FRONTEND_ONLY
  #define PRINT_FIELD(FIELDNAME) ;
  #define PRINT_MESSAGE(MESSAGE) ;
  #define DEBUG_1(TEXT) ;
#define DEBUG_2(TEXT,TEXT2);
#define DEBUG_3(TEXT,TEXT2,TEXT3);
#define DEBUG_4(TEXT,TEXT2,TEXT3,TEXT4);
#elif defined(NDEBUG)
  #define PRINT_FIELD(FIELDNAME) ;
  #define PRINT_MESSAGE(MESSAGE) ;
  #define PRINT_STRUCT_ARRAY(STRUCT,NO_OF_RECS) ;
  #define PRINT_STRUCT(STRUCT) ;
#else
  #define PRINT_FIELD(FIELDNAME) DEBUG_2( #FIELDNAME " : ", get##FIELDNAME ( ) ) 
  #define PRINT_STRUCT(STRUCT) STRUCT.print()
  #define PRINT_STRUCT_ARRAY(STRUCT,NO_OF_RECS) \
   { for( int i = 0 ; i < NO_OF_RECS ; i++ ) _##STRUCT[i].print() ; } 
#endif

#endif //UT_DEFINES_FOR_STRING_H
