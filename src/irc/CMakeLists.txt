file(GLOB sources_localdir *.cpp *.h)

SET(irc_STAT_SRCS
  ${irc_STAT_SRCS}
  ${sources_localdir}
)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/dep/include
  ${CMAKE_SOURCE_DIR}/src/framework
  ${CMAKE_SOURCE_DIR}/src/shared
  ${CMAKE_SOURCE_DIR}/src/shared/vmap
  ${CMAKE_SOURCE_DIR}/src/shared/Database
  ${CMAKE_SOURCE_DIR}/src/game
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${ACE_INCLUDE_DIR}
  ${MYSQL_INCLUDE_DIR}
)

add_library(irc STATIC ${irc_STAT_SRCS})

if(WIN32)
  target_link_libraries(irc
    ace
  )
else()
  target_link_libraries(irc
    ${ACE_LIBRARY}
  )
endif()

