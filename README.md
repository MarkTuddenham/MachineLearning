
# Teslyn

[![GitHub Actions](https://img.shields.io/endpoint.svg?url=https://actions-badge.atrox.dev/MarkTuddenham/Teslyn/badge&label=build&logo=none)](https://actions-badge.atrox.dev/MarkTuddenham/Teslyn/goto)
[![Coverage Status](https://coveralls.io/repos/github/MarkTuddenham/Teslyn/badge.svg?branch=master)](https://coveralls.io/github/MarkTuddenham/Teslyn?branch=master)
![GitHub](https://img.shields.io/github/license/marktuddenham/teslyn.svg)

![GitHub top language](https://img.shields.io/github/languages/top/marktuddenham/teslyn.svg)
![GitHub last commit](https://img.shields.io/github/last-commit/marktuddenham/teslyn.svg)
![GitHub issues](https://img.shields.io/github/issues/marktuddenham/teslyn.svg)
![GitHub closed issues](https://img.shields.io/github/issues-closed/marktuddenham/teslyn.svg)

A from-scratch implementation of Neural Networks.

---

## Build Process (Linux)

```bash
mkdir -p _build/

cmake -H. -B_build/
# or
cmake -H. -B_build/ -DCMAKE_BUILD_TYPE=Debug

# The above line may not work, if it doesn't then use:
pushd _build/ && cmake .. && popd

cmake --build _build/

# Run the tests
./bin/TeslynTests

# Run the example
./bin/TeslynExample
```

---
Copyright &copy; Mark Tuddenham
