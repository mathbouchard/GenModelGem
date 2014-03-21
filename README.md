GenModelGem
================

Ruby wrapper to Genmodel

LINUX
-----

Installation package requirements

* sudo apt-get install coinor-libcbc-dev
* sudo apt-get install coinor-libclp-dev
* sudo apt-get install coinor-libosi-dev
* sudo apt-get install coinor-libcgl-dev

Compiling package requirements

* sudo apt-get install build-essentials
* sudo apt-get install swig


MACOS
-----

Installation package requirements

* run brew doctor and make sure that you don't have any unlink packages and that you have all the necessary write permissions (according to brew doctor)
* cbc depends on : cloog gfortran gmp isl libmpc mpfr. They should be installed along cbc, but if some of them are already installed make sure they don't have any pending problem in brew doctor.
* brew tap homebrew/science
* brew tap dpo/coinor
* brew install cbc

Compiling package requirements

* Install Xcode
* brew install swig

To build:
---------

* cd <your_project_directory>
* git clone https://github.com/mathbouchard/GenModelGem.git
* cd Genmodel/
* swig -ruby -o ext/Genmodel/Genmodel.cpp -c++ src/Genmodel.i
* cd ext/Genmodel/
* ruby extconf.rb
* make
