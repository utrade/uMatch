# add_definitions( -DUDP_MARKET_DATA )
add_executable(uMatch
  orderBook.cpp
  market.cpp
  requestApplication.cpp
  replyApplication.cpp
  fixMarketDataHandler.cpp
  main.cpp
)

target_link_libraries(uMatch
  common
  pthread
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${QUICKFIX_LIBRARIES}
)
