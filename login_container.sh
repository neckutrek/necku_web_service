#!/bin/bash
#SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
docker exec -it -w /mount ctr_trading_application /bin/bash
