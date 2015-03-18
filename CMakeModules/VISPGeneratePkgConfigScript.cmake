#############################################################################
#
# $Id: VISPGeneratePkgConfigScript.cmake 5314 2015-02-12 08:32:30Z fspindle $
#
# This file is part of the ViSP software.
# Copyright (C) 2005 - 2014 by INRIA. All rights reserved.
# 
# This software is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# ("GPL") version 2 as published by the Free Software Foundation.
# See the file LICENSE.txt at the root directory of this source
# distribution for additional information about the GNU GPL.
#
# For using ViSP with software that can not be combined with the GNU
# GPL, please contact INRIA about acquiring a ViSP Professional 
# Edition License.
#
# See http://www.irisa.fr/lagadic/visp/visp.html for more information.
# 
# This software was developed at:
# INRIA Rennes - Bretagne Atlantique
# Campus Universitaire de Beaulieu
# 35042 Rennes Cedex
# France
# http://www.irisa.fr/lagadic
#
# If you have questions regarding the use of this file, please contact
# INRIA at visp@inria.fr
# 
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
# Description:
# This file generates the ViSP library config shell scripts: 
# - visp-config in <build dir>/bin from visp-config.in
# - visp-config in <build dir>/install from visp-config.install.in
#   When make install, this file is copied in <install dir>/bin
# - visp.pc in <build dir>/install from visp.pc.in
#   When make install, this file is copied in <install dir>/lib/pkgconfig
#
# Authors:
# Fabien Spindler
#
#############################################################################


if (UNIX)
  #######################################################################
  #
  # for Unix platforms: Linux, OSX
  #
  ####################################################################### 
  set(FILE_VISP_CONFIG_SCRIPT_IN "${VISP_SOURCE_DIR}/CMakeModules/visp-config.in")
  set(FILE_VISP_CONFIG_SCRIPT    "${BINARY_OUTPUT_PATH}/visp-config")

  set(FILE_VISP_CONFIG_SCRIPT_INSTALL_IN "${VISP_SOURCE_DIR}/CMakeModules/visp-config.install.in")
  set(FILE_VISP_CONFIG_SCRIPT_INSTALL    "${VISP_BINARY_DIR}/unix-install/visp-config")
  
  set(FILE_VISP_CONFIG_PC_INSTALL_IN "${VISP_SOURCE_DIR}/CMakeModules/visp.pc.in")
  set(FILE_VISP_CONFIG_PC_INSTALL    "${VISP_BINARY_DIR}/unix-install/visp.pc")

  #---------------------------------------------------------------------
  # Updates VISP_CONFIG_SCRIPT_PREFIX
  #----------------------------------------------------------------------
  set(VISP_CONFIG_SCRIPT_PREFIX "${CMAKE_INSTALL_PREFIX}")
 
  #---------------------------------------------------------------------
  # Updates VISP_CONFIG_CFLAGS
  #----------------------------------------------------------------------
  foreach(INCDIR ${VISP_EXTERN_INCLUDE_DIRS})
    list(APPEND VISP_CONFIG_CFLAGS "-I${INCDIR}")
  endforeach()

  # Suppress twins
  if(VISP_CONFIG_CFLAGS)
    list(REMOVE_DUPLICATES VISP_CONFIG_CFLAGS)
  endif()

  # Format the string to suppress CMake separators ";"
  set(VISP_CONFIG_CFLAGS_REFORMATED "")
  foreach(element ${VISP_CONFIG_CFLAGS})
    set(VISP_CONFIG_CFLAGS_REFORMATED "${VISP_CONFIG_CFLAGS_REFORMATED} ${element}")
  endforeach()
  set(VISP_CONFIG_CFLAGS ${VISP_CONFIG_CFLAGS_REFORMATED})
#  message(": ${VISP_CONFIG_CFLAGS}")

  if(BUILD_TEST_COVERAGE)
    # Add build options for test coverage. Currently coverage is only supported
    # on gcc compiler 
    # Because using -fprofile-arcs with shared lib can cause problems like:
    # hidden symbol `__bb_init_func', we add this option only for static 
    # library build
    set(VISP_CONFIG_CFLAGS "${VISP_CONFIG_CFLAGS} -ftest-coverage -fprofile-arcs")
  endif()

  #---------------------------------------------------------------------
  # Updates VISP_CONFIG_LIBS
  #
  # add "-l" to library names
  # skip *.so, *.a, *.dylib, -framework*, -l*
  #
  #----------------------------------------------------------------------

  # need to be improved
  if(POLICY CMP0026)
    cmake_policy(PUSH)
    cmake_policy(SET CMP0026 OLD)
    get_target_property(visp_libpath ${VISP_INTERN_LIBRARY} LOCATION_Release)
    cmake_policy(POP)
  else()
    get_target_property(visp_libpath ${VISP_INTERN_LIBRARY} LOCATION_Release)
  endif()
  #message("ViSP libpath: ${vip_libpath}")
  get_filename_component(visp_libname "${visp_libpath}" NAME)

  #message("ViSP libname: ${visp_libname}")
  # Manage the libs	
  list(REMOVE_ITEM VISP_EXTERN_LIBRARIES "debug")
  list(REMOVE_ITEM VISP_EXTERN_LIBRARIES "optimized")
  set(TMP_LIBS)
  foreach(lib ${VISP_EXTERN_LIBRARIES})
    if("${lib}" MATCHES "[-][f][r][a][m][e][w][o][r][k]+.")
      # does nothing
      list(APPEND TMP_LIBS ${lib})
    elseif("${lib}" MATCHES ".[.][f][r][a][m][e][w][o][r][k]+$")
      # replace /path/name.framework by -framework name
      get_filename_component(FRAMEWORK ${lib} NAME_WE)
      #message("add -framework ${FRAMEWORK}")
      list(APPEND TMP_LIBS "-framework ${FRAMEWORK}")
    elseif("${lib}" MATCHES ".[.][s][o]+$" OR "${lib}" MATCHES ".[.][a]+$")
      list(APPEND TMP_LIBS ${lib})
    elseif("${lib}" MATCHES ".[.][s][o][.][0123456789]+$")
      # does nothing
      list(APPEND TMP_LIBS ${lib})
    elseif("${lib}" MATCHES ".[.][s][o][.][0123456789]*[.][0123456789]+$")
      # does nothing
      list(APPEND TMP_LIBS ${lib})
    elseif("${lib}" MATCHES ".[.][s][o][.][0123456789]*[.][0123456789]*[.][0123456789]+$")
      # does nothing
      list(APPEND TMP_LIBS ${lib})
    elseif("${lib}" MATCHES ".[.][d][y][l][i][b]+$")
      # does nothing
      list(APPEND TMP_LIBS ${lib})
    elseif("${lib}" MATCHES "^(-l)")
      # does nothing
      list(APPEND TMP_LIBS ${lib})
    else()
      # add -l prefix
      #MESSAGE("add -l${lib}")
      list(APPEND TMP_LIBS "${lib}")
    endif()
  endforeach()

  foreach(val ${TMP_LIBS})
     set(VISP_CONFIG_LIBS "${VISP_CONFIG_LIBS} ${val}")
  endforeach(val)
 
  #---------------------------------------------------------------------
  # Updates the <build dir>/bin/visp-config shell script
  # Updates VISP_CONFIG_LIBS_SCRIPT (for visp-config)
  # Updates VISP_CONFIG_CFLAGS_SCRIPT (for visp-config)
  #----------------------------------------------------------------------

  # prepend with ViSP own include dir
  set(VISP_CONFIG_CFLAGS_SCRIPT "-I$PREFIX/${CMAKE_INSTALL_INCLUDEDIR} ${VISP_CONFIG_CFLAGS}")

  # prepend with ViSP own lib
  set(VISP_CONFIG_LIBS_SCRIPT  "$PREFIX/${CMAKE_INSTALL_LIBDIR}/${visp_libname} ${VISP_CONFIG_LIBS}")

  set(VISP_ECHO_NO_NEWLINE_CHARACTER "")
  set(VISP_ECHO_NO_NEWLINE_OPTION "")
  if(APPLE)
    set(VISP_ECHO_NO_NEWLINE_CHARACTER "\\c")
  else()
    set(VISP_ECHO_NO_NEWLINE_OPTION "-n")
  endif()

  configure_file(${FILE_VISP_CONFIG_SCRIPT_IN} ${FILE_VISP_CONFIG_SCRIPT})

  #---------------------------------------------------------------------
  # Updates the <build dir>/install/visp-config shell script
  #----------------------------------------------------------------------

  configure_file(${FILE_VISP_CONFIG_SCRIPT_INSTALL_IN} ${FILE_VISP_CONFIG_SCRIPT_INSTALL})

  #---------------------------------------------------------------------
  # Updates the <build dir>/install/visp.pc pkg-config file
  # Updates VISP_CONFIG_CFLAGS_PC (for libvisp.pc used by pkg-config)
  # Updates VISP_CONFIG_LIBS_PC (for libvisp.pc used by pkg-config)
  #----------------------------------------------------------------------
  set(exec_prefix "\${prefix}")
  set(includedir  "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
  set(libdir  "\${prefix}/${CMAKE_INSTALL_LIBDIR}")
 
  # prepend with ViSP own include dir
  set(VISP_CONFIG_CFLAGS_PC "-I\${includedir} ${VISP_CONFIG_CFLAGS}")

  # prepend with ViSP own lib dir and append -L<lib dir>
  set(VISP_CONFIG_LIBS_PC  "\${libdir}/${visp_libname} ${VISP_CONFIG_LIBS}")
  configure_file(${FILE_VISP_CONFIG_PC_INSTALL_IN} ${FILE_VISP_CONFIG_PC_INSTALL})

else(UNIX)
  #######################################################################
  #
  # for windows platforms
  #
  ####################################################################### 
  set(FILE_VISP_CONFIG_SCRIPT_IN "${VISP_SOURCE_DIR}/CMakeModules/visp-config.bat.in")
  set(FILE_VISP_CONFIG_SCRIPT    "${BINARY_OUTPUT_PATH}/visp-config.bat")
    
  set(FILE_VISP_CONFIG_SCRIPT_INSTALL_IN "${VISP_SOURCE_DIR}/CMakeModules/visp-config.bat.in")
  set(FILE_VISP_CONFIG_SCRIPT_INSTALL    "${VISP_BINARY_DIR}/win-install/visp-config-${VISP_ARCH}-${VISP_RUNTIME}.bat")

  #---------------------------------------------------------------------
  # Updates VISP_CONFIG_SCRIPT_PREFIX
  #----------------------------------------------------------------------
  set(VISP_CONFIG_SCRIPT_PREFIX "${CMAKE_INSTALL_PREFIX}")

  #---------------------------------------------------------------------
  # Updates VISP_CONFIG_SCRIPT_DEF
  #----------------------------------------------------------------------
  set(VISP_CONFIG_SCRIPT_DEFS "")
  set(VISP_OPENMP_SUPPORT "no")
  if(NOT ${VISP_OPENMP_FLAGS} STREQUAL "")
    set(VISP_CONFIG_SCRIPT_DEFS "${VISP_OPENMP_FLAGS}")
    set(VISP_OPENMP_SUPPORT "yes")
  endif()
  if(NOT ${VISP_CPP11_FLAGS} STREQUAL "")
    set(VISP_CONFIG_SCRIPT_DEFS "${VISP_CPP11_FLAGS}, ${VISP_CONFIG_SCRIPT_DEFS}")
  endif()

  #---------------------------------------------------------------------
  # Updates VISP_CONFIG_SCRIPT_INCLUDE
  #----------------------------------------------------------------------
  list(APPEND VISP_EXTERN_INCLUDE_DIRS "%PREFIX%/${CMAKE_INSTALL_INCLUDEDIR}")
  list(REMOVE_DUPLICATES VISP_EXTERN_INCLUDE_DIRS)

  # Format the string
  set(VISP_CONFIG_SCRIPT_INC ${VISP_EXTERN_INCLUDE_DIRS})
  #message(VISP_CONFIG_SCRIPT_INC ${VISP_CONFIG_SCRIPT_INC})

  #---------------------------------------------------------------------
  # Updates VISP_CONFIG_SCRIPT_LIBDIR
  # 1/ For usage with the build tree
  # 2/ For usage with the install tree
  #
  # and updates VISP_CONFIG_SCRIPT_LIBS_${config}
  #----------------------------------------------------------------------
  set(TMP_SCRIPT_LIBS_DEBUG "${VISP_INTERN_LIBRARY}${VISP_DLLVERSION}${VISP_DEBUG_POSTFIX}.lib")
  set(TMP_SCRIPT_LIBS_OPTIMIZED "${VISP_INTERN_LIBRARY}${VISP_DLLVERSION}.lib")

  #MESSAGE(VISP_EXTERN_LIBRARIES: ${VISP_EXTERN_LIBRARIES})
  set(TMP_IS_DEBUG FALSE)
  set(TMP_IS_OPTIMIZED FALSE)
  foreach(lib ${VISP_EXTERN_LIBRARIES})
    if("${lib}" MATCHES "[d][e][b][u][g]")
      set(TMP_IS_DEBUG TRUE)
    elseif("${lib}" MATCHES "[o][p][t][i][m][i][z][e][d]")
      set(TMP_IS_OPTIMIZED TRUE)
    else()
      # Get the library name
      get_filename_component(libname ${lib} NAME)
      if("${libname}" MATCHES ".+[.][l][i][b]" OR "${libname}" MATCHES ".+[.][L][i][b]")
        #MESSAGE("${libname} matches .lib or .Lib")
      else()
        # We need to add .lib suffix
        #MESSAGE("For ${libname} we add .lib suffix")
        set(libname "${libname}.lib")
      endif()

      # Get the library path
      get_filename_component(libpath ${lib} PATH)
      list(APPEND VISP_CONFIG_SCRIPT_LIBDIR_ "${libpath}")
      
      if(TMP_IS_DEBUG)
        set(TMP_IS_DEBUG FALSE)
        list(APPEND TMP_SCRIPT_LIBS_DEBUG ${libname})
      elseif(TMP_IS_OPTIMIZED)
        set(TMP_IS_OPTIMIZED FALSE)
        list(APPEND TMP_SCRIPT_LIBS_OPTIMIZED ${libname})
      else()
        list(APPEND TMP_SCRIPT_LIBS_DEBUG ${libname})
        list(APPEND TMP_SCRIPT_LIBS_OPTIMIZED ${libname})
      endif()
    endif()
  endforeach(lib)

  # Format the string
  set(VISP_CONFIG_SCRIPT_LIBS_DEBUG "${TMP_SCRIPT_LIBS_DEBUG}")
  set(VISP_CONFIG_SCRIPT_LIBS_OPTIMIZED "${TMP_SCRIPT_LIBS_OPTIMIZED}")

  # Format the string
  string(REGEX REPLACE "lib/Release" "lib/$(ConfigurationName)" VISP_CONFIG_SCRIPT_LIBDIR_ "${VISP_CONFIG_SCRIPT_LIBDIR_}")
  string(REGEX REPLACE "lib/Debug" "lib/$(ConfigurationName)" VISP_CONFIG_SCRIPT_LIBDIR_ "${VISP_CONFIG_SCRIPT_LIBDIR_}")

  # 1/ For usage with the build tree
  set(VISP_CONFIG_SCRIPT_LIBDIR "%PREFIX%/lib")
  list(APPEND VISP_CONFIG_SCRIPT_LIBDIR "%PREFIX%/lib/$(ConfigurationName)")
  list(APPEND VISP_CONFIG_SCRIPT_LIBDIR ${VISP_CONFIG_SCRIPT_LIBDIR_})
  list(REMOVE_DUPLICATES VISP_CONFIG_SCRIPT_LIBDIR)
  configure_file(${FILE_VISP_CONFIG_SCRIPT_IN} ${FILE_VISP_CONFIG_SCRIPT})

  # 2/ For usage with the install tree
  set(VISP_CONFIG_SCRIPT_LIBDIR "%PREFIX%/${VISP_ARCH}/${VISP_RUNTIME}/lib")
  list(APPEND VISP_CONFIG_SCRIPT_LIBDIR ${VISP_CONFIG_SCRIPT_LIBDIR_})
  list(REMOVE_DUPLICATES VISP_CONFIG_SCRIPT_LIBDIR)
  configure_file(${FILE_VISP_CONFIG_SCRIPT_INSTALL_IN} ${FILE_VISP_CONFIG_SCRIPT_INSTALL})
endif(UNIX)

#----------------------------------------------------------------------
# customize install target
#----------------------------------------------------------------------
# install rule for visp-config shell script
install(FILES ${FILE_VISP_CONFIG_SCRIPT_INSTALL}
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
  OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
  OWNER_WRITE
  COMPONENT libraries)
    
# install rule for visp.pc pkg-config file
if(UNIX)
  install(FILES ${FILE_VISP_CONFIG_PC_INSTALL}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    OWNER_WRITE
    COMPONENT libraries)
else()
  # not implemented yet
endif()


