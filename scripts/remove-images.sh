#!/usr/bin/env bash

for IMAGE in "web" "server"
do
  if [ ! -z "$(doctl registry repository list | grep "$(echo $IMAGE)")" ]
  then
    doctl registry repository dm $(echo $IMAGE) $(doctl registry repository lt $(echo $IMAGE) | grep -o "sha.*") --force
  fi
done
