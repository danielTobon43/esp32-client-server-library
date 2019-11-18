#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := esp32-library-modules
#EXTRA_COMPONENT_DIRS = ${CMAKE_SOURCE_DIR}/components

include $(IDF_PATH)/make/project.mk
include sdkconfig

