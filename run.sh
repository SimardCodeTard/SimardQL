#!/bin/bash
set -a
source .env
set +a
cd ./build
make
./SimardQL
