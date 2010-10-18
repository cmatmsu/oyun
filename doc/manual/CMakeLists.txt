
##########
# Get a list of the documentation source files
##########
file (GLOB DOC_SOURCE RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" source/*.text)
file (GLOB DOC_IMAGES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" source/*.png)


##########
# Generate TeX and HTML from Markdown
##########
set (TEX_FILES "${CMAKE_CURRENT_BINARY_DIR}/pdf/manual.tex")
set (TEX_IMAGES "")
set (HTML_FILES "")

foreach (doc ${DOC_SOURCE})
  get_filename_component (outfile ${doc} NAME_WE)
  
  add_custom_command (OUTPUT "html/${outfile}.html"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/html"
    COMMAND pandoc -f markdown -t html -s -o "${CMAKE_CURRENT_BINARY_DIR}/html/${outfile}.html" -S "${CMAKE_CURRENT_SOURCE_DIR}/${doc}"
    MAIN_DEPENDENCY "${doc}")
  add_custom_command (OUTPUT "pdf/${outfile}.tex"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/pdf"
    COMMAND pandoc -f markdown -t latex -o "${CMAKE_CURRENT_BINARY_DIR}/pdf/${outfile}.tex" "${CMAKE_CURRENT_SOURCE_DIR}/${doc}"
    MAIN_DEPENDENCY "${doc}")
  
  list (APPEND TEX_FILES "${CMAKE_CURRENT_BINARY_DIR}/pdf/${outfile}.tex")
  list (APPEND HTML_FILES "${CMAKE_CURRENT_BINARY_DIR}/html/${outfile}.html")
endforeach (doc)

# The main manual file is simply copied, but only if source and binary are different
if (NOT (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR}))
  add_custom_command (OUTPUT "pdf/manual.tex"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/pdf/manual.tex" "${CMAKE_CURRENT_BINARY_DIR}/pdf/manual.tex"
    MAIN_DEPENDENCY "pdf/manual.tex")
endif()


##########
# Copy images to binary directory
##########
foreach (image ${DOC_IMAGES})
  get_filename_component (outfile ${image} NAME)

  add_custom_command (OUTPUT "html/${outfile}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/${image}" "${CMAKE_CURRENT_BINARY_DIR}/html/${outfile}"
    MAIN_DEPENDENCY "${image}")
  add_custom_command (OUTPUT "pdf/${outfile}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/${image}" "${CMAKE_CURRENT_BINARY_DIR}/pdf/${outfile}"
    MAIN_DEPENDENCY "${image}")
  
  list (APPEND TEX_IMAGES "${CMAKE_CURRENT_BINARY_DIR}/pdf/${outfile}")
endforeach (image)


##########
# Build main TeX file
##########
if (WIN32 OR MSVC)
  set (NULDEV "NUL")
else()
  set (NULDEV "/dev/null")
endif()

add_custom_command (OUTPUT pdf/manual.pdf
  COMMAND pdflatex manual -interaction batchmode > "${NULDEV}" 2>&1
  COMMAND pdflatex manual -interaction batchmode > "${NULDEV}" 2>&1
  COMMAND pdflatex manual -interaction batchmode > "${NULDEV}" 2>&1
  DEPENDS ${TEX_FILES} ${TEX_IMAGES}
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/pdf/")


##########
# Copy wxHTML files to binary directory
##########
add_custom_command (OUTPUT html/logos-contents.hhc
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/wxhtml/logos-contents.hhc" "${CMAKE_CURRENT_BINARY_DIR}/html/logos-contents.hhc"
  MAIN_DEPENDENCY wxhtml/logos-contents.hhc)
add_custom_command (OUTPUT html/logos-index.hhk
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/wxhtml/logos-index.hhk" "${CMAKE_CURRENT_BINARY_DIR}/html/logos-index.hhk"
  MAIN_DEPENDENCY wxhtml/logos-index.hhk)
add_custom_command (OUTPUT html/logos.hhp
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/wxhtml/logos.hhp" "${CMAKE_CURRENT_BINARY_DIR}/html/logos.hhp"
  MAIN_DEPENDENCY wxhtml/logos.hhp)


##########
# Build the cached version of the HHP
##########
add_custom_command (OUTPUT html/logos.hhp.cached
  COMMAND hhp2cached "${CMAKE_CURRENT_BINARY_DIR}/html/logos.hhp"
  MAIN_DEPENDENCY "${CMAKE_CURRENT_BINARY_DIR}/html/logos.hhp"
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/html/logos-contents.hhc" "${CMAKE_CURRENT_BINARY_DIR}/html/logos-index.hhk" ${HTML_FILES})


##########
# Pack everything into an HTB
##########
add_custom_command (OUTPUT html/logos.htb
  COMMAND zip -q --quiet -j "logos.htb" ${HTML_FILES} logos-contents.hhc logos-index.hhk logos.hhp logos.hhp.cached
  DEPENDS html/logos.hhp.cached
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html")


##########
# Install the PDF and HTB manuals to $PREFIX/share or to the bundle on Mac
##########
if (APPLE)
  set (MANUAL_DESTINATION ".")
  set (DOCBOOK_DESTINATION "logos.app/Contents/Resources")
else ()
  set (MANUAL_DESTINATION "share/doc/logos")
  set (DOCBOOK_DESTINATION "share/logos")
endif ()

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/pdf/manual.pdf
  DESTINATION ${MANUAL_DESTINATION})
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/html/logos.htb
  DESTINATION ${DOCBOOK_DESTINATION})


add_custom_target (docs-manual ALL DEPENDS pdf/manual.pdf html/logos.htb)
