#!/bin/bash

# Zbiera wszystkie linie, wyciąga wartość clock i sortuje po niej
awk '
{
  match($0, /\(clock: ([0-9]+)\)/, arr)
  if (arr[1] != "") {
    print arr[1] "\t" $0
  }
}' build/logs/* | sort -n | cut -f2-
