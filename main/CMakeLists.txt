set(srcs "main.c"

		 "src/wifi_ap_mode.c"
		 "src/wifi_sta_module.c"
		 "src/post_data_cloud_module.c"
		 )

set(COMPONENT_ADD_INCLUDEDIRS .)

idf_component_register(SRCS ${srcs} INCLUDE_DIRS ".")




