#!/bin/bash
qmake6 || qmake-qt6
make clean &> /dev/null
rm -rf main.o mainwindow.o moc_mainwindow.cpp moc_mainwindow.o moc_predefs.h potatoMusic nohup.out clean
