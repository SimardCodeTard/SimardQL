#!/bin/bash
set -a
source private.env
source public.env
set +a
cd ./build
make
./SimardQL
