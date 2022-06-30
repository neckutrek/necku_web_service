#!/bin/bash
SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

case "$1" in
  "--debug"|"-d")
    dbgarg="-Dexec.args=\"-Xdebug\""
esac

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$SCRIPTPATH/market_connection/lib/
(cd trading_application && mvn exec:java -Dexec.mainClass=TradingApplication $dbgarg)

