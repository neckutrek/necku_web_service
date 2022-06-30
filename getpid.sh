#!/bin/bash
ps -ef | grep java | grep trading_application | awk '{print $2}'

