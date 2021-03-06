# Declares the dependencies of an application:
# Usage: DeclareAppDependencies( appTargetName [mrpt-xxx [mrpt-yyy] ...] )
#
macro(DeclareAppDependencies name)
	# Set app names:
	if(ENABLE_SOLUTION_FOLDERS)
		set_target_properties(${name} PROPERTIES FOLDER "applications")
	else(ENABLE_SOLUTION_FOLDERS)
		SET_TARGET_PROPERTIES(${name} PROPERTIES PROJECT_LABEL "(APP) ${name}")	
	endif(ENABLE_SOLUTION_FOLDERS)
	
	# set deps:
	set(ALL_DEPS "")
	
	FOREACH(DEP ${ARGN})
		LIST(APPEND ALL_DEPS ${DEP})
	ENDFOREACH(DEP)

	FOREACH(DEP ${ARGN})
		# Only for "mrpt-XXX" libs:
		IF (${DEP} MATCHES "mrpt-")
			get_property(LIB_DEP GLOBAL PROPERTY "${DEP}_LIB_DEPS")
			LIST(APPEND ALL_DEPS ${LIB_DEP})
		ENDIF (${DEP} MATCHES "mrpt-")
	ENDFOREACH(DEP)
	
	
	# Add the detected dependencies:
	list(REMOVE_DUPLICATES ALL_DEPS)
	IF (NOT "${ALL_DEPS}" STREQUAL "")
		#MESSAGE(STATUS "Adding deps: ${name} --> ${ALL_DEPS}")
		ADD_DEPENDENCIES(${name} ${ALL_DEPS})
		
		FOREACH (_DEP ${ALL_DEPS})
			# Link:
			IF(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
				get_property(_LIB_HDRONLY GLOBAL PROPERTY "${_DEP}_LIB_IS_HEADERS_ONLY")
				if (NOT _LIB_HDRONLY)
					TARGET_LINK_LIBRARIES(${name} ${_DEP}${MRPT_LINKER_LIBS_POSTFIX})
				endif (NOT _LIB_HDRONLY)
			ENDIF(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
			
			# Include:
			STRING(REGEX REPLACE "mrpt-(.*)" "\\1" DEP_MRPT_NAME ${_DEP})
			IF(NOT "${DEP_MRPT_NAME}" STREQUAL "")
				INCLUDE_DIRECTORIES("${MRPT_LIBS_ROOT}/${DEP_MRPT_NAME}/include/")
			ENDIF(NOT "${DEP_MRPT_NAME}" STREQUAL "")
		ENDFOREACH (_DEP)		
	ENDIF (NOT "${ALL_DEPS}" STREQUAL "")
	
endmacro(DeclareAppDependencies)

# Macro for adding links to the Start menu folder (for binary packages in Windows)
macro(AppStartMenuLink name title)
	get_property(_str GLOBAL PROPERTY "MRPT_CPACK_PACKAGE_EXECUTABLES")
	set_property(GLOBAL PROPERTY "MRPT_CPACK_PACKAGE_EXECUTABLES" "${_str}${name};${title};")
endmacro(AppStartMenuLink)


macro(DeclareAppForInstall name)
INSTALL(TARGETS ${name}
	RUNTIME DESTINATION ${mrpt_apps_INSTALL_PREFIX}bin COMPONENT Apps
	LIBRARY DESTINATION ${mrpt_apps_INSTALL_PREFIX}lib${LIB_SUFFIX} COMPONENT Apps
	ARCHIVE DESTINATION ${mrpt_apps_INSTALL_PREFIX}lib${LIB_SUFFIX} COMPONENT Apps)
endmacro(DeclareAppForInstall)

