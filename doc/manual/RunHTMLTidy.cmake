execute_process (COMMAND tidy -m --quiet 1 -b -u -wrap -omit -utf8 --output-html 1 --tidy-mark 0 --show-errors 0 --show-warnings 0 "${CMAKE_CURRENT_BINARY_DIR}/${basename}.html")
