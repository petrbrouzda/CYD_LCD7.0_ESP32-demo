#!/bin/bash
FONT_NAME=FrederickatheGreat-Regular.ttf
FONT_SIZE=30
FONT_WEIGHT=Medium
FONT_C_NAME=FrederickatheGreatRegular30

otf2bdf -w $FONT_WEIGHT -p $FONT_SIZE -r 96 -o font/$FONT_NAME-$FONT_SIZE.bdf font/$FONT_NAME
./bdfconv -v -f 1 -m "0-127,128-255,256-383" font/$FONT_NAME-$FONT_SIZE.bdf -o font/$FONT_C_NAME.h -n $FONT_C_NAME -d font/$FONT_NAME-$FONT_SIZE.bdf




