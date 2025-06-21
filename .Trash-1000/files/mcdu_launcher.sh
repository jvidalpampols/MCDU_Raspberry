#!/bin/bash

xfce4-terminal \
  --fullscreen \
  --hide-menubar \
  --hide-scrollbar \
  --geometry=24x14 \
  --font="Lucida Console Regular 19" \
  --command="/bin/bash -c '
    export LC_ALL=es_ES.UTF-8
    export LANG=es_ES.UTF-8
    sleep 0.2
    cd /mnt/mcdu/bin/
    sudo ./mcdu
    echo \"\nPresiona ENTER para salir...\"
    read'"
