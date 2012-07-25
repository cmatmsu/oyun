
# Look for dependencies
find_program (PANDOC pandoc)
find_program (PDFLATEX pdflatex)
find_program (ZIP zip)

##########
# First step: copy the images to the binary directory
##########
file (GLOB WIKI_IMAGES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" gh-wiki/images/*.png)
set (BINARY_DIR_IMAGES "")

foreach (image ${WIKI_IMAGES})
  get_filename_component (filename ${image} NAME)
  
  add_custom_command (OUTPUT "images/${filename}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/images"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/${image}" "${CMAKE_CURRENT_BINARY_DIR}/images/${filename}"
    MAIN_DEPENDENCY "${image}")
  
  list (APPEND BINARY_DIR_IMAGES "images/${filename}")
endforeach (image)


##########
# Second step: copy the TeX main-file over to the build directory
##########
set (TEX_DOCS "oyun-manual.tex")

add_custom_command (OUTPUT oyun-manual.tex
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/tools/oyun-manual.tex" "${CMAKE_CURRENT_BINARY_DIR}/oyun-manual.tex"
  MAIN_DEPENDENCY "tools/oyun-manual.tex")


##########
# Third step: copy the HTML utility files over to the build directory
##########
set (HTML_DOCS "oyun-contents.hhc;oyun-index.hhk;oyun.hhp")

add_custom_command (OUTPUT oyun-contents.hhc
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/tools/oyun-contents.hhc" "${CMAKE_CURRENT_BINARY_DIR}/oyun-contents.hhc"
  MAIN_DEPENDENCY tools/oyun-contents.hhc)
add_custom_command (OUTPUT oyun-index.hhk
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/tools/oyun-index.hhk" "${CMAKE_CURRENT_BINARY_DIR}/oyun-index.hhk"
  MAIN_DEPENDENCY tools/oyun-index.hhk)
add_custom_command (OUTPUT oyun.hhp
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/tools/oyun.hhp" "${CMAKE_CURRENT_BINARY_DIR}/oyun.hhp"
  MAIN_DEPENDENCY tools/oyun.hhp)


##########
# Fourth step: convert the gh-wiki Markdown into HTML and TeX in the build
# directory
##########
file (GLOB WIKI_DOCS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" gh-wiki/*.md)

foreach (doc ${WIKI_DOCS})
  get_filename_component (basename ${doc} NAME_WE)
  
  add_custom_command (OUTPUT "${basename}.html"
    COMMAND ${PANDOC} -f markdown -t html --template="${CMAKE_CURRENT_SOURCE_DIR}/tools/template.html" -o "${CMAKE_CURRENT_BINARY_DIR}/${basename}.html" "${CMAKE_CURRENT_SOURCE_DIR}/${doc}"
    COMMAND ${CMAKE_COMMAND} -D CMAKE_CURRENT_BINARY_DIR:string="${CMAKE_CURRENT_BINARY_DIR}" -D basename:string="${basename}" -P "${CMAKE_CURRENT_SOURCE_DIR}/RunHTMLTidy.cmake"
    COMMAND sed -i "" -e "\"s@<DIV id=.*>@@g\"" "${CMAKE_CURRENT_BINARY_DIR}/${basename}.html"
    COMMAND sed -i "" -e "\"s@</DIV>@@g\"" "${CMAKE_CURRENT_BINARY_DIR}/${basename}.html"
    MAIN_DEPENDENCY "${doc}")
  add_custom_command (OUTPUT "${basename}.tex"
    COMMAND ${PANDOC} -f markdown -t latex -o "${CMAKE_CURRENT_BINARY_DIR}/${basename}.tex" "${CMAKE_CURRENT_SOURCE_DIR}/${doc}"
    MAIN_DEPENDENCY "${doc}")
  
  list (APPEND TEX_DOCS "${basename}.tex")
  list (APPEND HTML_DOCS "${basename}.html")
endforeach (doc)


##########
# Fifth step: Build main TeX file
##########
if (WIN32 OR MSVC)
  set (NULDEV "NUL")
else()
  set (NULDEV "/dev/null")
endif()

add_custom_command (OUTPUT oyun-manual.pdf
  COMMAND ${PDFLATEX} oyun-manual -interaction batchmode > "${NULDEV}" 2>&1
  COMMAND ${PDFLATEX} oyun-manual -interaction batchmode > "${NULDEV}" 2>&1
  COMMAND ${PDFLATEX} oyun-manual -interaction batchmode > "${NULDEV}" 2>&1
  MAIN_DEPENDENCY oyun-manual.tex
  DEPENDS ${TEX_DOCS} ${BINARY_DIR_IMAGES})


##########
# 6. Build the cached version of the HHP
##########
add_custom_command (OUTPUT oyun.hhp.cached
  COMMAND hhp2cached "${CMAKE_CURRENT_BINARY_DIR}/oyun.hhp"
  MAIN_DEPENDENCY "${CMAKE_CURRENT_BINARY_DIR}/oyun.hhp"
  DEPENDS ${HTML_DOCS})


##########
# 7. Pack everything into an HTB
##########
add_custom_command (OUTPUT oyun.htb
  COMMAND ${ZIP} -q "oyun.htb" ${HTML_DOCS} ${BINARY_DIR_IMAGES} oyun.hhp.cached
  DEPENDS oyun.hhp.cached ${HTML_DOCS} ${BINARY_DIR_IMAGES})


##########
# Install the PDF and HTB manuals to $PREFIX/share or to the bundle on Mac
##########
if (APPLE)
  set (MANUAL_DESTINATION ".")
  set (DOCBOOK_DESTINATION "Oyun.app/Contents/Resources")
else ()
  set (MANUAL_DESTINATION "share/doc/oyun")
  set (DOCBOOK_DESTINATION "share/oyun")
endif ()

install (FILES "${CMAKE_CURRENT_BINARY_DIR}/oyun-manual.pdf"
  DESTINATION ${MANUAL_DESTINATION})
install (FILES "${CMAKE_CURRENT_BINARY_DIR}/oyun.htb"
  DESTINATION ${DOCBOOK_DESTINATION})


# Don't add the target unless we find 
if ((NOT (PANDOC STREQUAL "PANDOC-NOTFOUND")) AND
      (NOT (PDFLATEX STREQUAL "PDFLATEX-NOTFOUND")) AND
      (NOT (ZIP STREQUAL "ZIP-NOTFOUND"))) 
  add_custom_target (docs-manual ALL DEPENDS oyun-manual.pdf oyun.htb)
endif ()
