#!/usr/bin/env bash

IMAGE=server
if [ ! -z "$(doctl registry repository list | grep "$(echo $IMAGE)")" ]; then
  doctl registry repository dm $(echo $IMAGE) $(doctl registry repository lt $(echo $IMAGE) | grep -o "sha.*") --force
fi
