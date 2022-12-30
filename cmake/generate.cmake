# This file is a part of  cmake-template
# Copyright (C) 2022  akshay bansod <aksh.bansod@yahoo.com>

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.


# searches library in system, if not found builds from source
# searchLib(aliasName   [libraries...])
function(generate prjName)

    file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp)
    # exclude tests directory from chitra source
    list(FILTER sources EXCLUDE REGEX "${CMAKE_CURRENT_SOURCE_DIR}/tests/*")


    # setup project interface library
    add_library(i-${prjName} INTERFACE)
    target_sources(i-${prjName} INTERFACE ${sources})
    target_link_libraries(i-${prjName} INTERFACE ${libraries})
    target_include_directories(i-${prjName} INTERFACE ${CMAKE_SOURCE_DIR})
    target_precompile_headers(i-${prjName} INTERFACE pch.hpp)
    

    # add main executable
    add_executable(${prjName} ${sources} ${prjName}.cpp)

endfunction(generate prjName)
