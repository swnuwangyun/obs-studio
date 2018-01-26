project(simple-filters)

set(simple-filters_SOURCES
	volume-filter.c
	simple-filters-plugin-main.c)

add_library(simple-filters MODULE
	${simple-filters_SOURCES})
target_link_libraries(simple-filters
	libobs)

install_obs_plugin_with_data(simple-filters data)
