#!/bin/bash

g++ basic.cc -o basic `pkg-config --cflags --libs gtkmm-4.0` -std=c++17
