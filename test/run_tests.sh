#!/bin/bash

RN_EXECUTABLE=$(realpath ../build/RonaScript)
python3 functional_runner.py "$RN_EXECUTABLE"