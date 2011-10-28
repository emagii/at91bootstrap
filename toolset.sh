#!/bin/sh

if	[ "x$CROSS_COMPILE" == "x" ] ; then

	if	[ -f /usr/local/armv5te/bin/arm-angstrom-linux-gnueabi-gcc ] ; then
		export	CROSS_COMPILE=/usr/local/armv5te/bin/arm-angstrom-linux-gnueabi-
	elif	[ -f /usr/local/armv5te/bin/arm-angstrom-linux-gnueabi-gcc ] ; then
		export	CROSS_COMPILE=/usr/local/armv5te/bin/arm-angstrom-linux-gnueabi-
	else
		echo	"You need to set CROSS_COMPILE"
		exit	1
	fi
fi
echo	"Using CROSS_COMPILE=$CROSS_COMPILE"


