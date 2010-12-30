#!/bin/sh

rm -f ./sortie.lst
find -name "*.conf" -exec sed -rne '/^DisplayName/ { s/[^=]*=(.*)/\1/ ; p}; /^UniqueId/ {s/[^=]*=(.*)/\1/ ; p}' {} >> sortie.lst \;
sed -re 'N; s/[\n\r]+/:/' sortie.lst | sed -re 's/^([^:]*):(.*)$/\2:\1/' | sort -n  > files_list.lst

