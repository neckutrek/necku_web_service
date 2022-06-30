#!/bin/bash
docker exec -it -w /mount ctr_trading_application bash -c 'gdb -p $(./getpid.sh)'

