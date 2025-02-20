set(CMAKE_C_COMPILER "/opt/software/software/GCCcore/11.2.0/bin/cc")
set(CMAKE_C_COMPILER_ARG1 "")
set(CMAKE_C_COMPILER_ID "GNU")
set(CMAKE_C_COMPILER_VERSION "11.2.0")
set(CMAKE_C_COMPILER_VERSION_INTERNAL "")
set(CMAKE_C_COMPILER_WRAPPER "")
set(CMAKE_C_STANDARD_COMPUTED_DEFAULT "17")
set(CMAKE_C_COMPILE_FEATURES "c_std_90;c_function_prototypes;c_std_99;c_restrict;c_variadic_macros;c_std_11;c_static_assert;c_std_17;c_std_23")
set(CMAKE_C90_COMPILE_FEATURES "c_std_90;c_function_prototypes")
set(CMAKE_C99_COMPILE_FEATURES "c_std_99;c_restrict;c_variadic_macros")
set(CMAKE_C11_COMPILE_FEATURES "c_std_11;c_static_assert")
set(CMAKE_C17_COMPILE_FEATURES "c_std_17")
set(CMAKE_C23_COMPILE_FEATURES "c_std_23")

set(CMAKE_C_PLATFORM_ID "Linux")
set(CMAKE_C_SIMULATE_ID "")
set(CMAKE_C_COMPILER_FRONTEND_VARIANT "")
set(CMAKE_C_SIMULATE_VERSION "")




set(CMAKE_AR "/opt/software/software/binutils/2.37-GCCcore-11.2.0/bin/ar")
set(CMAKE_C_COMPILER_AR "/opt/software/software/GCCcore/11.2.0/bin/gcc-ar")
set(CMAKE_RANLIB "/opt/software/software/binutils/2.37-GCCcore-11.2.0/bin/ranlib")
set(CMAKE_C_COMPILER_RANLIB "/opt/software/software/GCCcore/11.2.0/bin/gcc-ranlib")
set(CMAKE_LINKER "/opt/software/software/binutils/2.37-GCCcore-11.2.0/bin/ld")
set(CMAKE_MT "")
set(CMAKE_COMPILER_IS_GNUCC 1)
set(CMAKE_C_COMPILER_LOADED 1)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_C_ABI_COMPILED TRUE)
set(CMAKE_COMPILER_IS_MINGW )
set(CMAKE_COMPILER_IS_CYGWIN )
if(CMAKE_COMPILER_IS_CYGWIN)
  set(CYGWIN 1)
  set(UNIX 1)
endif()

set(CMAKE_C_COMPILER_ENV_VAR "CC")

if(CMAKE_COMPILER_IS_MINGW)
  set(MINGW 1)
endif()
set(CMAKE_C_COMPILER_ID_RUN 1)
set(CMAKE_C_SOURCE_FILE_EXTENSIONS c;m)
set(CMAKE_C_IGNORE_EXTENSIONS h;H;o;O;obj;OBJ;def;DEF;rc;RC)
set(CMAKE_C_LINKER_PREFERENCE 10)

# Save compiler ABI information.
set(CMAKE_C_SIZEOF_DATA_PTR "8")
set(CMAKE_C_COMPILER_ABI "ELF")
set(CMAKE_C_BYTE_ORDER "LITTLE_ENDIAN")
set(CMAKE_C_LIBRARY_ARCHITECTURE "")

if(CMAKE_C_SIZEOF_DATA_PTR)
  set(CMAKE_SIZEOF_VOID_P "${CMAKE_C_SIZEOF_DATA_PTR}")
endif()

if(CMAKE_C_COMPILER_ABI)
  set(CMAKE_INTERNAL_PLATFORM_ABI "${CMAKE_C_COMPILER_ABI}")
endif()

if(CMAKE_C_LIBRARY_ARCHITECTURE)
  set(CMAKE_LIBRARY_ARCHITECTURE "")
endif()

set(CMAKE_C_CL_SHOWINCLUDES_PREFIX "")
if(CMAKE_C_CL_SHOWINCLUDES_PREFIX)
  set(CMAKE_CL_SHOWINCLUDES_PREFIX "${CMAKE_C_CL_SHOWINCLUDES_PREFIX}")
endif()





set(CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES "/opt/software/software/FFTW/3.3.10-gompi-2021b/include;/opt/software/software/FlexiBLAS/3.0.4-GCC-11.2.0/include;/opt/software/software/OpenBLAS/0.3.18-GCC-11.2.0/include;/opt/software/software/OpenMPI/4.1.1-GCC-11.2.0/include;/opt/software/software/PMIx/4.1.0-GCCcore-11.2.0/include;/opt/software/software/libfabric/1.13.2-GCCcore-11.2.0/include;/opt/software/software/UCX/1.11.2-GCCcore-11.2.0/include;/opt/software/software/libevent/2.1.12-GCCcore-11.2.0/include;/opt/software/software/hwloc/2.5.0-GCCcore-11.2.0/include;/opt/software/software/libpciaccess/0.16-GCCcore-11.2.0/include;/opt/software/software/libxml2/2.9.10-GCCcore-11.2.0/include/libxml2;/opt/software/software/libxml2/2.9.10-GCCcore-11.2.0/include;/opt/software/software/numactl/2.0.14-GCCcore-11.2.0/include;/opt/software/software/binutils/2.37-GCCcore-11.2.0/include;/opt/software/software/libarchive/3.5.1-GCCcore-11.2.0/include;/opt/software/software/XZ/5.2.5-GCCcore-11.2.0/include;/opt/software/software/cURL/7.78.0-GCCcore-11.2.0/include;/opt/software/software/OpenSSL/1.1/include;/opt/software/software/bzip2/1.0.8-GCCcore-11.2.0/include;/opt/software/software/zlib/1.2.11-GCCcore-11.2.0/include;/opt/software/software/ncurses/6.2-GCCcore-11.2.0/include;/opt/software/software/googletest/1.11.0-GCCcore-11.3.0/include;/opt/software/software/gettext/0.21.1-GCCcore-12.3.0/include;/opt/software/software/expat/2.5.0-GCCcore-12.3.0/include;/opt/software/software/GCCcore/11.2.0/lib/gcc/x86_64-pc-linux-gnu/11.2.0/include;/opt/software/software/GCCcore/11.2.0/include;/opt/software/software/GCCcore/11.2.0/lib/gcc/x86_64-pc-linux-gnu/11.2.0/include-fixed;/usr/include")
set(CMAKE_C_IMPLICIT_LINK_LIBRARIES "gcc;gcc_s;c;gcc;gcc_s")
set(CMAKE_C_IMPLICIT_LINK_DIRECTORIES "/opt/software/software/ScaLAPACK/2.1.0-gompi-2021b-fb/lib64;/opt/software/software/FFTW/3.3.10-gompi-2021b/lib64;/opt/software/software/FlexiBLAS/3.0.4-GCC-11.2.0/lib64;/opt/software/software/OpenBLAS/0.3.18-GCC-11.2.0/lib64;/opt/software/software/OpenMPI/4.1.1-GCC-11.2.0/lib64;/opt/software/software/PMIx/4.1.0-GCCcore-11.2.0/lib64;/opt/software/software/libfabric/1.13.2-GCCcore-11.2.0/lib64;/opt/software/software/UCX/1.11.2-GCCcore-11.2.0/lib64;/opt/software/software/libevent/2.1.12-GCCcore-11.2.0/lib64;/opt/software/software/hwloc/2.5.0-GCCcore-11.2.0/lib64;/opt/software/software/libpciaccess/0.16-GCCcore-11.2.0/lib64;/opt/software/software/libxml2/2.9.10-GCCcore-11.2.0/lib64;/opt/software/software/numactl/2.0.14-GCCcore-11.2.0/lib64;/opt/software/software/binutils/2.37-GCCcore-11.2.0/lib64;/opt/software/software/libarchive/3.5.1-GCCcore-11.2.0/lib64;/opt/software/software/XZ/5.2.5-GCCcore-11.2.0/lib64;/opt/software/software/cURL/7.78.0-GCCcore-11.2.0/lib64;/opt/software/software/OpenSSL/1.1/lib64;/opt/software/software/bzip2/1.0.8-GCCcore-11.2.0/lib64;/opt/software/software/zlib/1.2.11-GCCcore-11.2.0/lib64;/opt/software/software/ncurses/6.2-GCCcore-11.2.0/lib64;/opt/software/software/googletest/1.11.0-GCCcore-11.3.0/lib64;/opt/software/software/Perl/5.36.1-GCCcore-12.3.0/lib64;/opt/software/software/gettext/0.21.1-GCCcore-12.3.0/lib64;/opt/software/software/expat/2.5.0-GCCcore-12.3.0/lib64;/opt/software/software/code-server/4.16.1/lib64;/opt/software/software/GCCcore/11.2.0/lib/gcc/x86_64-pc-linux-gnu/11.2.0;/opt/software/software/GCCcore/11.2.0/lib64;/lib64;/usr/lib64;/opt/software/software/ScaLAPACK/2.1.0-gompi-2021b-fb/lib;/opt/software/software/FFTW/3.3.10-gompi-2021b/lib;/opt/software/software/FlexiBLAS/3.0.4-GCC-11.2.0/lib;/opt/software/software/OpenBLAS/0.3.18-GCC-11.2.0/lib;/opt/software/software/OpenMPI/4.1.1-GCC-11.2.0/lib;/opt/software/software/PMIx/4.1.0-GCCcore-11.2.0/lib;/opt/software/software/libfabric/1.13.2-GCCcore-11.2.0/lib;/opt/software/software/UCX/1.11.2-GCCcore-11.2.0/lib;/opt/software/software/libevent/2.1.12-GCCcore-11.2.0/lib;/opt/software/software/hwloc/2.5.0-GCCcore-11.2.0/lib;/opt/software/software/libpciaccess/0.16-GCCcore-11.2.0/lib;/opt/software/software/libxml2/2.9.10-GCCcore-11.2.0/lib;/opt/software/software/numactl/2.0.14-GCCcore-11.2.0/lib;/opt/software/software/binutils/2.37-GCCcore-11.2.0/lib;/opt/software/software/libarchive/3.5.1-GCCcore-11.2.0/lib;/opt/software/software/XZ/5.2.5-GCCcore-11.2.0/lib;/opt/software/software/cURL/7.78.0-GCCcore-11.2.0/lib;/opt/software/software/OpenSSL/1.1/lib;/opt/software/software/bzip2/1.0.8-GCCcore-11.2.0/lib;/opt/software/software/zlib/1.2.11-GCCcore-11.2.0/lib;/opt/software/software/ncurses/6.2-GCCcore-11.2.0/lib;/opt/software/software/googletest/1.11.0-GCCcore-11.3.0/lib;/opt/software/software/Perl/5.36.1-GCCcore-12.3.0/lib;/opt/software/software/gettext/0.21.1-GCCcore-12.3.0/lib;/opt/software/software/expat/2.5.0-GCCcore-12.3.0/lib;/opt/software/software/code-server/4.16.1/lib;/opt/software/software/GCCcore/11.2.0/lib")
set(CMAKE_C_IMPLICIT_LINK_FRAMEWORK_DIRECTORIES "")
