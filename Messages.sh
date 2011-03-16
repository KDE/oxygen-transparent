#! /usr/bin/env bash
$EXTRACTRC `find . -name \*.ui -o -name \*.kcfg` >> rc.cpp
$XGETTEXT `find . -name \*.cpp -o -name \*.h` -o $podir/oxygen_transparent.pot
