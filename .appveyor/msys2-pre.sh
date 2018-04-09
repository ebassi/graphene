#!/bin/bash

set -e

# Disable slow diskspace check
sed -i 's/^CheckSpace/#CheckSpace/g' /etc/pacman.conf

# System upgrade
pacman --noconfirm -Syyuu
