#!/bin/sh

glslc shaders/basic.vert -o shaders/basic_vert.spv
glslc shaders/basic.frag -o shaders/basic_frag.spv
cp shaders/basic_vert.spv build/shaders/
cp shaders/basic_frag.spv build/shaders/
