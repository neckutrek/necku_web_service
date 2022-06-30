#!/bin/bash

SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
docker rmi $(docker images --filter "dangling=true" -q)
cp $SCRIPTPATH/trading_application/pom.xml $SCRIPTPATH/docker/pom.xml && cd $SCRIPTPATH/docker && docker build . -t java8-dbg:latest
docker rmi $(docker images --filter "dangling=true" -q)

