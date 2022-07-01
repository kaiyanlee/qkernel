#/usr/bin/sh

# Format all .cc files.

find -name "*.cc" | clang-format
