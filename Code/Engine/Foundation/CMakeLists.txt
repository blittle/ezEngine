#cmake

#
# Copy this file into the folder of an engine library to add it to the solution.
# This can be a regular engine library, or an experimental library.
# Below there are configuration options to add preprocessor definitions and link dependencies to the library.
#
# *** Do not use this file for 'Experimental' libraries ***
#
# *** Modify this file as required ***
#

# Add all the preprocessor definitions that are needed to configure this library
# add_definitions (-DMY_PREPROCESS_DEFINITION_1)
# add_definitions (-DMY_PREPROCESS_DEFINITION_2)

# Add all the libraries that this project needs to link against here
SET (LINK_LIBRARIES
  ThirdParty
 )

if (MSVC)
  #link_directories(${CMAKE_SOURCE_DIR}/Code/ThirdParty/gpa_ITT/libs/$(PlatformName))

  SET (LINK_LIBRARIES
    ${LINK_LIBRARIES}
    ${CMAKE_SOURCE_DIR}/Code/ThirdParty/gpa_ITT/libs/$(PlatformName)/gpasdk_d.lib
  )
endif()


if (CURRENT_OSX_VERSION)
  FIND_LIBRARY(CORESERVICES_LIBRARY CoreServices )
  FIND_LIBRARY(COREFOUNDATION_LIBRARY CoreFoundation )
  
  MARK_AS_ADVANCED (CORESERVICES_LIBRARY
                    COREFOUNDATION_LIBRARY)

  SET (LINK_LIBRARIES
    ${LINK_LIBRARIES}
    ${CORESERVICES_LIBRARY}
	${COREFOUNDATION_LIBRARY}
  )
endif()

include (${CMAKE_SOURCE_DIR}/Code/BuildSystem/CMake/CMAKE_Library.txt)