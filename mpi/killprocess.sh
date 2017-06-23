#!/bin/bash

ps -efww|grep $1|grep -v grep|cut -c 9-15|xargs kill -9
