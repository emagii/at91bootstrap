#!/bin/sh
find board -depth -print | grep  defconfig | sed -e "s/board\///g"	> board/boards

