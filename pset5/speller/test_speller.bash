#! /bin/bash

./speller $1 > test/my_out
~cs50/pset5/speller $1 > test/their_out

diff test/my_out test/their_out | tee > test/diff
