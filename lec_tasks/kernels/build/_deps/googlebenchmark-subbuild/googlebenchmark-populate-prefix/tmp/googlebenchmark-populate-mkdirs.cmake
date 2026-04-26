# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/mnt/c/users/1/desktop/max_folder/inst/tensor_compiler/lec_tasks/kernels/build/_deps/googlebenchmark-src"
  "/mnt/c/users/1/desktop/max_folder/inst/tensor_compiler/lec_tasks/kernels/build/_deps/googlebenchmark-build"
  "/mnt/c/users/1/desktop/max_folder/inst/tensor_compiler/lec_tasks/kernels/build/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix"
  "/mnt/c/users/1/desktop/max_folder/inst/tensor_compiler/lec_tasks/kernels/build/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/tmp"
  "/mnt/c/users/1/desktop/max_folder/inst/tensor_compiler/lec_tasks/kernels/build/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/src/googlebenchmark-populate-stamp"
  "/mnt/c/users/1/desktop/max_folder/inst/tensor_compiler/lec_tasks/kernels/build/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/src"
  "/mnt/c/users/1/desktop/max_folder/inst/tensor_compiler/lec_tasks/kernels/build/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/src/googlebenchmark-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/mnt/c/users/1/desktop/max_folder/inst/tensor_compiler/lec_tasks/kernels/build/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/src/googlebenchmark-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/mnt/c/users/1/desktop/max_folder/inst/tensor_compiler/lec_tasks/kernels/build/_deps/googlebenchmark-subbuild/googlebenchmark-populate-prefix/src/googlebenchmark-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
