# CMakeList.txt : CMake project for QtStaticExample, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.20)

function(download_archive url dir sha256)
	set(URL_ARG ${url})
	cmake_path(GET URL_ARG FILENAME archive_file_name)
	string(REPLACE ".tar.xz" "" archive_dir_name ${archive_file_name})

	file(TO_CMAKE_PATH ${CMAKE_BINARY_DIR}/${dir}/${archive_file_name} archive_file_path)
	file(TO_CMAKE_PATH ${CMAKE_BINARY_DIR}/${dir}/${archive_dir_name} archive_dir_path)
	
	if(EXISTS ${archive_dir_path})
		message(STATUS "Archive already extracted to ${archive_dir_path}")
	else()
		message(STATUS "Downloading archive ${url} to ${archive_file_path}")
		file(DOWNLOAD ${url} ${archive_file_path}
			EXPECTED_HASH SHA256=${sha256}
			SHOW_PROGRESS
		)
		message(STATUS "Extracting...")
		file(ARCHIVE_EXTRACT INPUT ${archive_file_path} DESTINATION ${archive_dir_path})
		message(STATUS "Archive ${archive_file_name} extracted to ${archive_dir_path}")
	endif()
endfunction()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("QtStaticExample")

set(QT_VERSION_MAJOR 6.7)
set(QT_VERSION_MINOR 6.7.0)
set(QT_URL https://download.qt.io/official_releases/qt/${QT_VERSION_MAJOR}/${QT_VERSION_MINOR}/single/qt-everywhere-src-${QT_VERSION_MINOR}.tar.xz)
set(QT_INSTALL_DIR ${CMAKE_BINARY_DIR}/dependencies/qt6)
download_archive(${QT_URL} dependencies "bf5089912364f99cf9baf6c109de76a3172eec6267f148c69800575c47f90087")
execute_process(COMMAND configure -static -prefix ${CMAKE_BINARY_DIR}/dependencies/qt-everywhere-build-${QT_VERSION_MINOR} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/dependencies/qt-everywhere-src-${QT_VERSION_MINOR}/qt-everywhere-src-${QT_VERSION_MINOR})
execute_process(COMMAND cmake . WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/dependencies/qt-everywhere-src-${QT_VERSION_MINOR}/qt-everywhere-src-${QT_VERSION_MINOR})
execute_process(COMMAND cmake --build . --parallel WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/dependencies/qt-everywhere-src-${QT_VERSION_MINOR}/qt-everywhere-src-${QT_VERSION_MINOR})
execute_process(COMMAND cmake --install . WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/dependencies/qt-everywhere-src-${QT_VERSION_MINOR}/qt-everywhere-src-${QT_VERSION_MINOR})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets HINTS ${QT_INSTALL_DIR}/lib/cmake/Qt6)

add_executable(
	QtStaticExample
	"app/main.cpp" 
	"app/MainWindow.cpp"
)

target_link_libraries(QtStaticExample PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)

set_target_properties(QtStaticExample PROPERTIES
    WIN32_EXECUTABLE ON
)

add_dependencies(QtStaticExample qt6)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET QtStaticExample PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
