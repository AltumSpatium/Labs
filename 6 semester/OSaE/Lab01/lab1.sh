#!/bin/bash

{ echo "$1"; hexdump < "$1"; whoami; date; } > res;
