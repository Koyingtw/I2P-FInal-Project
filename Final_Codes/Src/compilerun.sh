#!/bin/bash

if [ $# -gt 1 ] ; then
    gcc $1.c -o $1.out $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_image-5 allegro_primitives-5 allegro_ttf-5 --libs --cflags) $2 && ./$1.out
else
    gcc $1.c -o $1.out $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_image-5 allegro_primitives-5 allegro_ttf-5 --libs --cflags)  && ./$1.out
fi

