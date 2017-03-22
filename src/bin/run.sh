#!/bin/bash

killall -9 ./strade_runner.out
rm -f nohup.out
nohup ./strade_runner.out &
