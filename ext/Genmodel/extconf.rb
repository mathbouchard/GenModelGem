require 'mkmf'
require 'rbconfig'
require 'find'

extension_name = 'Genmodel/Genmodel'
dir_config(extension_name)

#abort "missing malloc()" unless have_func "malloc"
#abort "missing free()"   unless have_func "free"

parts = RUBY_DESCRIPTION.split(' ')
type = parts[0]
type = type[4..-1] if type.start_with?('tcs-')
type = 'ree' if 'ruby' == type && RUBY_DESCRIPTION.include?('Ruby Enterprise Edition')
is_windows = RbConfig::CONFIG['host_os'] =~ /(mingw|mswin)/
is_linux = RbConfig::CONFIG['host_os'] =~ /(linux)/
is_darwin = RbConfig::CONFIG['host_os'] =~ /(darwin??)/
platform = RUBY_PLATFORM
version = RUBY_VERSION.split('.')


if is_darwin
    puts "DEFINE Darwin"
    $defs.push("-DDarwin")
    elsif is_linux
    puts "DEFINE Linux"
    $defs.push("-DLinux")
    elsif is_windows
    puts "DEFINE WIN64"
    $defs.push("-DWIN64")
    else
    puts "Unknown platform"
    $defs.push("-DUnknownPlatform")
end
puts ">>>>> Creating Makefile for #{type} version #{RUBY_VERSION} on #{platform} (#{RbConfig::CONFIG['host_os']}) <<<<<"


puts "#{CONFIG["dldflags"]}"
CONFIG["DLDFLAGS"].gsub!(/-multiply_definedsuppress/, '')
$DLDFLAGS = CONFIG["DLDFLAGS"]
puts "#{CONFIG["DLDFLAGS"]}, #{$DLDFLAGS}"

puts CONFIG['CPPOUTFILE']

CONFIG["CFLAGS"] = '-O2 -Wall -pthread -fmessage-length=0 -fPIC -std=c++0x'
#CONFIG["CXX"] = 'g++'

sep = File::PATH_SEPARATOR
is_cplex = true;
is_osi = true;
if is_darwin
    path = Dir.home+"/Applications/IBM/ILOG/CPLEX_Studio126/cplex/include/"
    puts "Looking for ilcplex/cplex.h in "+path
    if(is_cplex && find_header("ilcplex/cplex.h",path))
        puts "found"
    else
        puts "not found"
        is_cplex = false
    end
    
    path = Dir.home+"/Applications/IBM/ILOG/CPLEX_Studio126/cplex/lib/x86-64_osx/static_pic"
    puts "Looking for libcplex (function CPXopenCPLEX) in "+path
    if(is_cplex && find_library("cplex","CPXopenCPLEX",path))  #"CPXopenCPLEX",path))
        puts "found"
    else
        puts "not found"
        is_cplex = false
    end
    
    #path = "/usr/local/Cellar/cbc/2.8.6/include/coin/"+sep+"/usr/local/Cellar/cgl/0.58.3/include/coin/"
    cbc_path = "/usr/local/Cellar/cbc/"
    cbc_file = "/include/coin/OsiClpSolverInterface.hpp"
    puts "Looking for OsiClpSolverInterface.hpp in "+cbc_path+"/*"
    file_exist = nil
    Find.find(cbc_path) do |path|
        if (FileTest.directory?(path))
            puts path+cbc_file
            temp_name = (path).gsub(/\/usr\/local\/Cellar\/cbc\//,'')
            count = temp_name.count('/')
            if ((count == 0) && (File.exist?(path+cbc_file)))
                file_exist = "-I#{path}/include/coin"
                break
            end
        end
    end
    if(is_osi && file_exist != nil)
        $INCFLAGS << " " << file_exist.quote
        puts "found"
    else
        puts "not found"
        is_osi = false
    end
    
    path = "/usr/lib:/usr/local/lib/"
    puts "Looking for Clp (function main) in "+path
    if(is_osi && find_library("Clp",nil,path))
        puts "found"
    else
        puts "not found"
        is_osi = false
    end
    
    path = "/usr/lib:/usr/local/lib/"
    puts "Looking for Cbc (function main) in "+path
    if(is_osi && find_library("Cbc",nil,path))
        puts "found"
    else
        puts "not found"
        is_osi = false
    end
    
    path = "/usr/lib:/usr/local/lib/"
    puts "Looking for Cgl (function main) in "+path
    if(is_osi && find_library("Cgl",nil,path))
        puts "found"
    else
        puts "not found"
        is_osi = false
    end
    
    path = "/usr/lib:/usr/local/lib/"
    puts "Looking for Osi (function main) in "+path
    if(is_osi && find_library("Osi",nil,path))
        puts "found"
    else
        puts "not found"
        is_osi = false
    end
    
    path = "/usr/lib:/usr/local/lib/"
    puts "Looking for OsiClp (function main) in "+path
    if(is_osi && find_library("OsiClp",nil,path))
        puts "found"
    else
        puts "not found"
        is_osi = false
    end
    
    
    found_coin_utils = false
    path = "/usr/lib:/usr/local/lib/"
    puts "Looking for CoinUtils (function main) in "+path
    if(is_osi && find_library("CoinUtils",nil,path))
        found_coin_utils = true
    end
    coin_utils_path = "/usr/lib/"
    coin_utils_file = "libCoinUtils.so"
    if(!found_coin_utils && is_osi && File.exist?(coin_utils_path+coin_utils_file))
        puts "Fallback : Looking for libCoinUtils.so in "+coin_utils_path
        found_coin_utils = true
    end
    coin_utils_path = "/usr/local/lib/"
    if(!found_coin_utils && is_osi && File.exist?(coin_utils_path+coin_utils_file))
        puts "Second fallback : Looking for libCoinUtils.so in "+coin_utils_path
        found_coin_utils = true
    end
    if found_coin_utils
        puts "found"
        $LIBS << "-lCoinUtils"
    else
        puts "not found"
        is_osi = false
    end
    
    
elsif is_linux
    # path = Dir.home+"/ibm/ILOG/CPLEX_Studio126/cplex/include:/opt/ibm/ILOG/CPLEX_Studio126/cplex/include"
    path = "/opt/ibm/ILOG/CPLEX_Studio126/cplex/include"
    puts "Looking for ilcplex/cplex.h in "+path
    if(is_cplex && find_header("ilcplex/cplex.h",path))
        puts "found"
    else
        puts "not found"
        is_cplex = false
    end
    
    path = "/usr/lib:/usr/local/lib/"
    puts "Looking for pthread (function main) in "+path
    if(find_library("pthread",nil,path))
        puts "found"
    else
        puts "not found"
    end
    
    cbc_path = "/usr/include/coin/"
    cbc_file = "OsiClpSolverInterface.hpp"
    puts "Looking for OsiClpSolverInterface.hpp in "+cbc_path+"/*"
    if(is_osi && File.exist?(cbc_path+cbc_file))
        $INCFLAGS << " -I" << cbc_path.quote
        puts "found"
    else
        puts "not found"
        is_osi = false
    end
    
    path = "/usr/lib:/usr/local/lib/"
    puts "Looking for Clp (function main) in "+path
    if(is_osi && find_library("Clp",nil,path))
        puts "found"
    else
        puts "not found"
        is_osi = false
    end
    
    path = "/usr/lib:/usr/local/lib/"
    puts "Looking for Cbc (function main) in "+path
    if(is_osi && find_library("Cbc",nil,path))
        puts "found"
    else
        puts "not found"
        is_osi = false
    end
    
    path = "/usr/lib:/usr/local/lib/"
    puts "Looking for Cgl (function main) in "+path
    if(is_osi && find_library("Cgl",nil,path))
        puts "found"
    else
        puts "not found"
        is_osi = false
    end
    
    path = "/usr/lib:/usr/local/lib/"
    puts "Looking for Osi (function main) in "+path
    if(is_osi && find_library("Osi",nil,path))
        puts "found"
    else
        puts "not found"
        is_osi = false
    end
    
    path = "/usr/lib:/usr/local/lib/"
    puts "Looking for OsiClp (function main) in "+path
    if(is_osi && find_library("OsiClp",nil,path))
        puts "found"
    else
        puts "not found"
        is_osi = false
    end
    
    found_coin_utils = false
    path = "/usr/lib:/usr/local/lib/"
    puts "Looking for CoinUtils (function main) in "+path
    if(is_osi && find_library("CoinUtils",nil,path))
        found_coin_utils = true
    end
    coin_utils_path = "/usr/lib/"
    coin_utils_file = "libCoinUtils.so"
    if(!found_coin_utils && is_osi && File.exist?(coin_utils_path+coin_utils_file))
        puts "Fallback : Looking for libCoinUtils.so in "+coin_utils_path
        found_coin_utils = true
    end
    coin_utils_path = "/usr/local/lib/"
    if(!found_coin_utils && is_osi && File.exist?(coin_utils_path+coin_utils_file))
        puts "Second fallback : Looking for libCoinUtils.so in "+coin_utils_path
        found_coin_utils = true
    end
    if found_coin_utils
        puts "found"
        $LIBS << "-lCoinUtils"
    else
        puts "not found"
        is_osi = false
    end
else
    is_cplex = false;
    is_osi = false;
end


if(is_cplex)
    puts "*** With CPLEX_MOUDLE ***"
    #$defs.push("-DCPLEX_MODULE")
end
if(is_osi)
    puts "*** With OSI_MOUDLE ***"
    $defs.push("-DOSI_MODULE")
end



create_makefile (extension_name)
