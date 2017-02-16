#!/bin/sh

devdocs_url="https://developer.gnome.org"

dir=`pwd`

for file in $dir/*.html; do
  echo "Fixing cross-references in ${file}..."
  sed -i \
        -e "s|/usr/share/gtk-doc/html/gobject|${devdocs_url}/gobject/stable|" \
        -e "s|/usr/share/gtk-doc/html/glib|${devdocs_url}/glib/stable|" \
        -e "s|\.\./glib|${devdocs_url}/glib/stable|" \
        -e "s|\.\./gobject|${devdocs_url}/gobject/stable|" \
  ${file}
done
