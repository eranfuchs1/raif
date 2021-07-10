#!/bin/bash



install_dir="/usr/bin/";

if [ ! -z "$2" ]; then
	install_dir="$2";
fi

if [ -e $1 ]; then
	for dir in $(ls .)
	do
		if [ -d "$dir" ]; then
			cd "$dir";
			make;
			cd ..;
		fi
	done
elif [ "$1" == "install" ]; then
	for dir in $(ls .)
	do
		if [ -z "$(echo "$dir" | grep '^raif_')" ]; then
			continue
		fi
		if [ -d "$dir" ]; then
			cd "$dir";
			make install 2=$install_dir;
			cd ..;
		fi
	done
fi
