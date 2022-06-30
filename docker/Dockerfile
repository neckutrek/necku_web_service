
# My very own docker file

FROM docker.io/library/ubuntu:latest
MAINTAINER neckutrek

RUN apt update -y
RUN apt upgrade -y
RUN apt install -y gdb openjdk-8-jdk maven openjdk-8-jre vim
RUN apt autoremove -y

COPY pom.xml /pom.xml
RUN mvn dependency:resolve

