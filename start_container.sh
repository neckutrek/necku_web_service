#!/bin/bash
SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
docker run -d -it --name ctr_trading_application -v $SCRIPTPATH:/mount java8-dbg:latest sleep infinity

