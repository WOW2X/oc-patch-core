add_subdirectory(updates)
add_subdirectory(tools)
add_subdirectory(scripts)
add_subdirectory(oc-patch)


########### install files ###############

FILE(GLOB _SQL *.sql)

install(FILES ${_SQL} DESTINATION share/oregon/sql)
