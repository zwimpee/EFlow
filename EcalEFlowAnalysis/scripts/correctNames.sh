#!/bin/tcsh

foreach file ( filelist*txt )
#    echo $file | sed -e "s%filelist%filelist %g" | awk '{printf "mv -f %s%s %sRUN2011%s\n",$1,$2,$1,$2}' | /bin/sh
    echo $file | sed -e "s%filelist%filelist %g" | awk '{printf "mv -f %s%s %s2011Plus2012%s\n",$1,$2,$1,$2}' | /bin/sh
end          
