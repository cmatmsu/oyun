Undertake the following steps before completing a release:

1. Years and Dates
    * Update Doxygen date (`doc/doxygen/mainpages/main.h`): month and year
    * Update year if it's changed:
        * `ChangeLog` files in root directory
        * `README`
        * All source-file copyrights in `src/`
        * Copyright notices in the [[Release Notes]]
2. Release Notes
    * Update the [[Release Notes]] and generate the NEWS file by running `make news_file`.
    * Change version numbers in `CMakeLists.txt` and in the `Doxyfile`.
3. Documentation
    * Check that any and all new features are documented in the [[documentation|Home]].
    * Update the docs in the master branch by executing: `cd doc/manual/gh-wiki`; `git pull`; `cd ..`; `git add gh-wiki` and committing.
    * Check that a Doxygen documentation run doesn't report any errors. (**FIXME:** We're currently being bitten by a Doxygen bug, which will remain until Doxygen 1.7.2 comes out.  Re-run documentation then.)
    * Build the Doxygen docs and copy them over to the `gh-pages` branch.
4. Commit Code
    * Push the proposed final version and test it on Windows and Mac.  Also, build all three installation packages and ensure that they function correctly.
    * Tag the version on GitHub: `git tag -a v2.0.b2`.
    * From this point on, *no* changes to the code.
5. Build Packages
    * Using the tagged release, build Windows, Mac, and Source packages.
    * Upload the packages to GitHub.
6. Announce
    * Update all the links on the [[Downloads]] page.
    * Update all the links on the [[Home]] page.
    * **FIXME:** When we have a mailing list, send an e-mail to the logos-announce list.