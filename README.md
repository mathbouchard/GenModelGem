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
