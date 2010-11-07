Prerequisites
------------------

To compile logos, you need:

* Your platform's development tools
    * Windows: Visual C++, most likely any version after 6.0, including Visual C Express
    * Mac: Xcode (installed from your OS X DVDs, or from [[Apple's Xcode website|http://developer.apple.com/technologies/tools/xcode.html]]
    * Linux: The standard GCC/development tools (Ubuntu: `sudo apt-get install build-essential`)
* [[CMake|http://www.cmake.org/]], at least v2.8
    * Windows: Download and install from the CMake website
    * Mac: Either download and install from CMake, or `sudo port install cmake`
    * Linux: Install through your distribution (Ubuntu: `sudo apt-get install cmake`; Gentoo: `sudo emerge cmake`)
* [[wxWidgets|http://www.wxwidgets.org/]], at least v2.9
    * Windows: Download the wxWidgets self extracting executable from the wxWidgets website, then build it.  **NOTE:** The current version of wxWidgets (2.9.1) cannot be built out-of-the-box on Visual C++ 10.  You will need project files from [[this forum post|http://forums.wxwidgets.org/viewtopic.php?t=27630]].
    * Mac: Download the source package from wxWidgets, and configure with `arch_flags="-arch x86_64 " configure --with-cocoa --disable-shared --disable-compat24 --enable-unicode --enable-universal_binary --with-macosx-sdk=/Developer/SDKs/MacOSX10.5.sdk CFLAGS="$arch_flags" CXXFLAGS="$arch_flags" CPPFLAGS="$arch_flags" LDFLAGS="$arch_flags" OBJCFLAGS="$arch_flags" OBJCXXFLAGS="$arch_flags"`.  This enables three-arch (ppc, i386, x86_64) universal static libraries, which is what we need to build distributions.  Compile and install.
    * Linux: Most linux distributions aren't yet shipping wxWidgets 2.9 (an exception: this package can be installed on Gentoo if you're using ~ARCH packages).  Download the source package from wxWidgets and execute the standard `configure`/`make`/`make install` combo.  Make sure to pass `--enable-unicode` to `configure`.
* If you wish to build either the documentation or distribution packages, you will also need:
    * [[pandoc|http://johnmacfarlane.net/pandoc/]]
        * Windows: Install the self-extracting installer from pandoc.
        * Mac: `sudo port install pandoc`
        * Linux: Install through your distribution (Ubuntu: `sudo apt-get install pandoc`, Gentoo: `sudo emerge pandoc`)
    * A full TeX install, preferably [[TeX Live|http://tug.org/texlive/]]
        * Windows/Mac: Install the TeX Live distribution from its website
        * Linux: Install through your distribution (Ubuntu: `sudo apt-get install texlive`, Gentoo: `sudo emerge texlive`)
    * zip
        * Windows: Install [[Cygwin|http://www.cygwin.com/]], making sure to install the "zip" package.  Place the path to your Cygwin `bin` directory in your system PATH.
        * Mac: `sudo port install zip`
        * Linux: Probably already installed through your distribution (Ubuntu: `sudo apt-get install zip`, Gentoo: `sudo emerge zip`)

Compiling
--------------

First, you need to unpack the Logos source archive and create a build directory:

    tar xvf logos-VERSION.tar.bz2
    mkdir build
    cd build

Now, run CMake to generate project/build files for your platform:

    cmake ../logos-VERSION

On Windows, you will need to specify a generator:

    cmake -G "Visual Studio 2010" ../logos-VERSION

An aside: if you wish to compile universal binaries on Mac (e.g., for distribution), after the first run of CMake, open the CMakeCache.txt file and change the value of `CMAKE_OSX_ARCHITECTURES` to `ppc;i386;x86_64`.  Also, for maximum compatibility you'll want to set `CMAKE_OSX_DEPLOYMENT_TARGET` to `10.5` and change `CMAKE_OSX_SYSROOT` to point to the 10.5 SDK.

If CMake runs successfully (and finds wxWidgets), you now just need to build Logos.  On Linux and Mac, simply execute `make`.  On Windows, open the generated solution file (Logos.sln or Logos.dsw) in Visual Studio and compile.  The Logos binaries will be located in your build tree.  On Linux, you may simply execute `make install` to install them.  On Windows and Mac, you probably want to run `make package`, which will create an installer file (logos-VERSION-win.exe or logos-VERSION-mac.dmg) which you can use to properly install Logos.