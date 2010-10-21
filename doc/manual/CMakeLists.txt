
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
set (TEX_DOCS "logos-manual.tex")

add_custom_command (OUTPUT logos-manual.tex
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/tools/logos-manual.tex" "${CMAKE_CURRENT_BINARY_DIR}/logos-manual.tex"
  MAIN_DEPENDENCY "tools/logos-manual.tex")


##########
# Third step: copy the HTML utility files over to the build directory
##########
set (HTML_DOCS "logos-contents.hhc;logos-index.hhk;logos.hhp")

add_custom_command (OUTPUT logos-contents.hhc
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/tools/logos-contents.hhc" "${CMAKE_CURRENT_BINARY_DIR}/logos-contents.hhc"
  MAIN_DEPENDENCY tools/logos-contents.hhc)
add_custom_command (OUTPUT logos-index.hhk
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/tools/logos-index.hhk" "${CMAKE_CURRENT_BINARY_DIR}/logos-index.hhk"
  MAIN_DEPENDENCY tools/logos-index.hhk)
add_custom_command (OUTPUT logos.hhp
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/tools/logos.hhp" "${CMAKE_CURRENT_BINARY_DIR}/logos.hhp"
  MAIN_DEPENDENCY tools/logos.hhp)


##########
# Fourth step: convert the gh-wiki Markdown into HTML and TeX in the build
# directory
##########
file (GLOB WIKI_DOCS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" gh-wiki/*.md)

foreach (doc ${WIKI_DOCS})
  get_filename_component (basename ${doc} NAME_WE)
  
  add_custom_command (OUTPUT "${basename}.html"
    COMMAND pandoc -f markdown -t html --template="${CMAKE_CURRENT_SOURCE_DIR}/tools/template.html" -o "${CMAKE_CURRENT_BINARY_DIR}/${basename}.html" "${CMAKE_CURRENT_SOURCE_DIR}/${doc}"
    COMMAND ${CMAKE_COMMAND} -D CMAKE_CURRENT_BINARY_DIR:string="${CMAKE_CURRENT_BINARY_DIR}" -D basename:string="${basename}" -P "${CMAKE_CURRENT_SOURCE_DIR}/RunHTMLTidy.cmake"
    COMMAND sed -i "" -e "\"s@<DIV id=.*>@@g\"" "${CMAKE_CURRENT_BINARY_DIR}/${basename}.html"
    COMMAND sed -i "" -e "\"s@</DIV>@@g\"" "${CMAKE_CURRENT_BINARY_DIR}/${basename}.html"
    MAIN_DEPENDENCY "${doc}")
  add_custom_command (OUTPUT "${basename}.tex"
    COMMAND pandoc -f markdown -t latex -o "${CMAKE_CURRENT_BINARY_DIR}/${basename}.tex" "${CMAKE_CURRENT_SOURCE_DIR}/${doc}"
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

add_custom_command (OUTPUT logos-manual.pdf
  COMMAND pdflatex logos-manual -interaction batchmode > "${NULDEV}" 2>&1
  COMMAND pdflatex logos-manual -interaction batchmode > "${NULDEV}" 2>&1
  COMMAND pdflatex logos-manual -interaction batchmode > "${NULDEV}" 2>&1
  MAIN_DEPENDENCY logos-manual.tex
  DEPENDS ${TEX_DOCS} ${BINARY_DIR_IMAGES})


##########
# 6. Build the cached version of the HHP
##########
add_custom_command (OUTPUT logos.hhp.cached
  COMMAND hhp2cached "${CMAKE_CURRENT_BINARY_DIR}/logos.hhp"
  MAIN_DEPENDENCY "${CMAKE_CURRENT_BINARY_DIR}/logos.hhp"
  DEPENDS ${HTML_DOCS})


##########
# 7. Pack everything into an HTB
##########
add_custom_command (OUTPUT logos.htb
  COMMAND zip -q --quiet "logos.htb" ${HTML_DOCS} ${BINARY_DIR_IMAGES} logos.hhp.cached
  DEPENDS logos.hhp.cached ${HTML_DOCS} ${BINARY_DIR_IMAGES})


##########
# Install the PDF and HTB manuals to $PREFIX/share or to the bundle on Mac
##########
if (APPLE)
  set (MANUAL_DESTINATION ".")
  set (DOCBOOK_DESTINATION "Logos.app/Contents/Resources")
else ()
  set (MANUAL_DESTINATION "share/doc/logos")
  set (DOCBOOK_DESTINATION "share/logos")
endif ()

install (FILES "${CMAKE_CURRENT_BINARY_DIR}/logos-manual.pdf"
  DESTINATION ${MANUAL_DESTINATION})
install (FILES "${CMAKE_CURRENT_BINARY_DIR}/logos.htb"
  DESTINATION ${DOCBOOK_DESTINATION})


add_custom_target (docs-manual ALL DEPENDS logos-manual.pdf logos.htb)
