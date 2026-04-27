#!/bin/bash

# stop script on error
set -e

# output colors
GREEN='\033[0-32m'
BLUE='\033[0-34m'
NC='\033[0m' # No Color

echo -e "${BLUE}=== Запуск автоматической сборки проекта kernels ===${NC}"

# remove old build dir
if [ -d "build" ]; then
    echo -e "${BLUE}Очистка старой директории build...${NC}"
    rm -rf build
fi

mkdir build && cd build

# cmake config
# give flag SSE, cause proc does not handle AVX2
echo -e "${BLUE}project config with handling of SSE4.2...${NC}"
cmake -DCMAKE_BUILD_TYPE=Release ..

echo -e "${BLUE}compilation (using $(nproc) of threads)...${NC}"
make -j$(nproc)

echo -e "${GREEN}=== testing ===${NC}"
./unit_tests

echo -e "${GREEN}=== benchmarking ===${NC}"
./bench_kernels --benchmark_format=console --benchmark_out=../benchmark_results.json

echo -e "${BLUE}=== done! ===${NC}"
echo -e "benchmark results saved to${GREEN}benchmark_results.json${NC}"
cd ..