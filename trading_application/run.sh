#!/bin/bash

LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/git/necku_web_service/market_connection/lib/ mvn exec:java -Dexec.mainClass=TradingApplication

