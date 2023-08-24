![image](https://github.com/TRRXITTE/traaitt/blob/master/docs/XTE.png)

#### traaitt [XTE] is a fast, and easy way to send money to friends and businesses

### Compiling traaitt [XTE]

##### Table of Contents

-   **Linux**
    -   [Linux Dependencies](#linux-dependencies)
    -   [Ubuntu with GCC](#ubuntu-with-gcc)
    -   [Ubuntu with CLANG](#ubuntu-with-clang)
    -   [CentOS with GCC](#centos-with-gcc)
    -   [Generic Linux](#generic-linux)

##### Build Optimization

The CMake build system will, by default, create optimized _native_ builds for your particular system type when you build the software. Using this method, the binaries created provide a better experience and all-together faster performance.

##### Making Portable Binaries

However, if you wish to create _portable_ binaries that can be shared between systems, specify `-DARCH=default` in your CMake arguments during the build process. Note that _portable_ binaries will have a noticable difference in performance than _native_ binaries. For this reason, it is always best to build for your particular system if possible.

## Linux

##### Linux Dependencies

**Note:** Individual names for these packages may differ depending on your Linux distribution.

-   [Boost](https://www.boost.org/)
-   [OpenSSL](https://www.openssl.org/)
-   [Cmake (3.8, higher)](https://cmake.org/download/)
-   [GNU Make](https://ftp.gnu.org/gnu/make/)
-   [Git](https://git-scm.com/)
-   [GCC 7, higher](https://gcc.gnu.org/) or [CLANG 6, higher](https://clang.llvm.org/)

##### Ubuntu with GCC

-   `sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y`
-   `sudo apt-get update`
-   `sudo apt-get install aptitude -y`
-   `sudo aptitude install -y build-essential g++-8 gcc-8 git libboost-all-dev python-pip libssl-dev`
-   `sudo pip install cmake`
-   `export CC=gcc-8`
-   `export CXX=g++-8`
-   `git clone -b master --single-branch https://github.com/TRRXITTE/traaitt`
-   `cd traaitt`
-   `mkdir build`
-   `cd build`
-   `cmake ..`
-   `make`

The binaries will be in the `src` folder when you are complete.

-   `cd src`
-   `./XTEnetwork --version`

##### Ubuntu with CLANG

###### For Ubuntu 16.04 (Xenial) users:

-   `sudo add-apt-repository "deb https://apt.llvm.org/xenial/ llvm-toolchain-xenial 6.0 main"`

###### For Ubuntu 18.04 (Bionic) users:

-   `sudo add-apt-repository "deb https://apt.llvm.org/bionic/ llvm-toolchain-bionic 6.0 main"`
-   `sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y`
-   `wget -O - https://apt.llvm.org/Developer Tools.llvm-snapshot.gpg.key | sudo apt-key add -`

*   `sudo apt-get update`
*   `sudo apt-get install aptitude -y`
*   `sudo aptitude install -y build-essential clang-6.0 libstdc++-7-dev git libboost-all-dev python-pip libssl-dev`
*   `sudo pip install cmake`
*   `export CC=clang-6.0`
*   `export CXX=clang++-6.0`
*   `git clone -b master --single-branch https://github.com/TRRXITTE/traaitt`
*   `cd traaitt`
*   `mkdir build`
*   `cd build`
*   `cmake ..`
*   `make`

###### For Ubuntu 20.04 (focal) users:

*   `wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -`
*   `sudo add-apt-repository "deb http://apt.llvm.org/focal/ llvm-toolchain-focal main`
*   `wget https://apt.llvm.org/llvm.sh`
*   `chmod +x llvm.sh`
*   `sudo ./llvm.sh 16`
*   `sudo apt-get update`
*   `sudo apt-get install aptitude -y`
*   `sudo aptitude install -y build-essential libstdc++-7-dev git libboost-all-dev libssl-dev cmake`
*   `sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-16 100`
*   `sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-16 100`
*   `git clone -b master --single-branch https://github.com/TRRXITTE/traaitt`
*   `cd traaitt`
*   `mkdir build`
*   `cd build`
*   `cmake ..`
*   `make`


The binaries will be in the `src` folder when you are complete.

-   `cd src`
-   `./XTEnetwork --version`

##### CentOS with GCC

_CentOS compile instructions provided by @brandonlehmann_

-   `sudo yum update -y`
-   `sudo yum install -y epel-release centos-release-scl`
-   `sudo yum install -y devtoolset-8 cmake3 wget git openssl-devel`
-   `sudo scl enable devtoolset-8 bash`
-   `wget https://dl.bintray.com/boostorg/release/1.68.0/source/boost_1_68_0.tar.gz`
-   `tar xzvf boost_1_68_0.tar.gz && cd boost_1_68_0`
-   `./bootstrap.sh --prefix=/usr/local/`
-   `./b2 -j$(nproc) -d0 install --with-system --with-filesystem --with-thread --with-date_time --with-chrono --with-regex --with-serialization --with-program_options`
-   `cd ..`
-   `git clone https://github.com/TRRXITTE/traaitt`
-   `mkdir -p traaitt/build`
-   `cd traaitt/build`
-   `cmake3 ..`
-   `make`

The binaries will be in the `src` folder when you are complete.

-   `cd src`
-   `./XTEnetwork --version`

##### Generic Linux

**Note:** If you want to use clang, ensure you set the environment variables `CC` and `CXX`.
See the ubuntu instructions for an example.

-   `git clone -b master --single-branch https://github.com/TRRXITTE/traaitt`
-   `cd traaitt`
-   `mkdir build`
-   `cd build`
-   `cmake ..`
-   `make`

The binaries will be in the `src` folder when you are complete.

-   `cd src`
-   `./XTEnetwork --version`

[^ Return To Top](#compile-unix)