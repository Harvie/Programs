#!/bin/bash
DATASET='set_small.dat.bz2'
#DATASET='set_large.dat.bz2'

OUTDIR='/tmp/eset';
mkdir -p $OUTDIR;
PREPARED="$OUTDIR/test";

#prepare data
bzip2 -dc "$DATASET" | cut -d ' ' -f 2- | sort -u > "$PREPARED"


echo runing...

