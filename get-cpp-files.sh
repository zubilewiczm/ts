#!/bin/bash
find "$1" -type f -iname '*.cpp' -printf '%h\0%d\0%P\n' | sort -t '\0' -n | awk -F '\0' '{print $3}'
