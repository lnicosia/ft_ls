#!/bin/bash

OPT="$@"

echo "$file diff:"
./ft_ls $OPT .. > ft_ls.txt
ls $OPT .. > ls.txt
diff ft_ls.txt ls.txt
#rm ft_ls.txt ls.txt
