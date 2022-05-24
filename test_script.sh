#!/bin/bash

OPT="$@"

./ft_ls $OPT > ft_ls.txt
echo "$?" > ft_ls-ret.txt
ls $OPT > ls.txt
echo "$?" > ls-ret.txt
echo "Output diff:"
diff ft_ls.txt ls.txt
echo "Return diff:"
diff ft_ls-ret.txt ls-ret.txt
#rm ft_ls.txt ls.txt
