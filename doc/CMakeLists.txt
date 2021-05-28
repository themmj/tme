find_program(DOT_FOUND dot)
if (DOT_FOUND)
    set(HAVE_DOT "YES")
else()
    set(HAVE_DOT "NO")
endif()

find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/doc/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc-doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else ()
  message("Doxygen need to be installed to generate the doxygen documentation")
endif ()

find_program(HPP2PUML_FOUND hpp2plantuml)
if (HPP2PUML_FOUND)
    set(PUML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/puml)
    add_custom_target(doc-puml ALL
        COMMAND mkdir -p ${PUML_DIR}
        COMMAND ./generate-plant-uml
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

    find_program(PLANTUML_FOUND plantuml.jar)
    if (PLANTUML_FOUND)
        find_path(PLANTUML_PATH plantuml.jar)
        set(SVG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/svg)
        add_custom_target(doc-puml-svg ALL
            COMMAND mkdir -p ${SVG_DIR}
            COMMAND ./generate-plant-uml-svg ${PLANTUML_PATH}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    else()
        message("plantuml.jar needs to be installed to generate SVG files from PUML files of the codebase")
    endif()

else()
    message("hpp2plantuml needs to be installed to generate PUML of the codebase")
endif()

