#!/bin/bash
if [ -d "/Library/Audio/Plug-Ins/Components/$1" ]; then
	sudo rm -r "/Library/Audio/Plug-Ins/Components/$1"
fi
sudo mv "$2/Products/Development/$1" "/Library/Audio/Plug-Ins/Components/"
