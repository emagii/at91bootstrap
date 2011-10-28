#!/bin/sh
export	CONFIG_LIST=board/allboards.cfg

mk_config ()
{
	rm	-f	$CONFIG_LIST
	for path in `ls	board/*/*_defconfig` ; do
#		echo	$path
#		echo	`basename $path`
		filename=`basename $path | sed s/_defconfig//g`
		echo	"$filename" >> $CONFIG_LIST
	done
}

mk_config

# cat	$CONFIG_LIST

