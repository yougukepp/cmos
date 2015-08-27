#!/bin/sh

rm -rf *.pdf
asy 2dxoyrotate.asy
evince 2dxoyrotate.pdf

