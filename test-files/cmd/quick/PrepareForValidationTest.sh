#!/bin/bash
set -e

export TA_IN=test-files/conflate/point/Poi1.osm
export TA_STDIR=test-files/prepare_validation
export TA_OUT=test-output/cmd/quick/prepare_validation
mkdir -p $TA_OUT

CONFIG="-C Testing.conf"

NODE="$(command -v nodejs || command -v node)"
$NODE $HOOT_HOME/scripts/services/PrepareForValidation.js $TA_IN $TA_OUT/poi1_validation.osm false
hoot diff $CONFIG --ignore-uuid $TA_OUT/poi1_validation.osm $TA_STDIR/poi1_validation.osm

