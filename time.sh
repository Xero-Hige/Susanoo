#!/bin/bash
START=$(date +%s)
# do something
# start your script work here
./Susanoo -d ./ArchivosPrueba
# your logic ends here
END=$(date +%s)
DIFF=$(( $END - $START ))
echo "Tomo $DIFF segundos"
