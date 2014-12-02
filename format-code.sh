#!/bin/bash
if [ "$#" -eq 0 ]; then
    astyle --suffix=none --options=.astylerc --recursive ./*.h ./*.cpp
else
    astyle --suffix=none --options=.astylerc $@
fi
