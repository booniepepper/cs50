#! /bin/bash

make resize

./resize 2 smiley.bmp smilier.bmp
./resize 3 smiley.bmp smiliest.bmp

diff smiley.bmp smilier.bmp > smiley_diff_2x.txt
diff smiley.bmp smiliest.bmp > smiley_diff_3x.txt
