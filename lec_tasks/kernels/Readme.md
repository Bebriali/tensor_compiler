# kernels
### lecture task for 27.04.26

task implementing GEMM and CONV operations on matrixes.

## how to run
### using docker
build docker app

```
docker build -t tensor_compiler .
```

then run benchmarking

```
docker run --rm tensor_compiler
```

or tests

```
docker run --rm tensor_compiler unit_tests
```
### using bash
make it executable
```
chmod +x run.sh
```
then run
```
./run.sh
```

## results
saved in benchmark_results.json and represented in the terminal