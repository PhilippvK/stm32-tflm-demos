#!/bin/bash

DATASET_DIR=$1
OUTPUT_DIR=$2
COUNT=$3

function print_usage() {
  echo 'Usage: ./gen_mnist_files.sh [DATASET_DIR] [OUTPUT_DIR] [COMMANDS] [COUNT]'
}

if [[ -z $DATASET_DIR ]]
then
  echo "DATASET_DIR not defined! Aborting..."
  print_usage
  exit 1
else
  if [[ ! -d $DATASET_DIR ]]
  then
    echo "DATASET_DIR does not exist!"
    exit 1
  fi
fi

if [[ -z $OUTPUT_DIR ]]
then
  echo "OUTPUT_DIR not defined! Aborting..."
  print_usage
  exit 1
else
  if [[ ! -d $OUTPUT_DIR ]]
  then
    echo "OUTPUT_DIR does not exist!"
    exit 1
  fi
fi

if [[ -z $COUNT ]]
then
  echo "COUNT not defined! Aborting..."
  print_usage
  exit 1
else
  if [[ ! $COUNT -ge 0 ]]
  then
    echo "COUNT has to be a positive integer! Aborting..."
    exit 1
  fi
fi

AVAILABLE_BMP_FILES=(`find $DATASET_DIR -iname '*.bmp' -exec basename {} \;`)
#CHOSEN_COMMANDS=(`echo $COMMANDS | tr , "\n"`)


#echo $AVAILABLE_BMP_FILES

for i in $(seq $COUNT)
do
  RAND=`expr $RANDOM`
  RANDOM_FILE=${AVAILABLE_BMP_FILES[$RAND % ${#AVAILABLE_BMP_FILES[@]}]}
  FULL_FILE_NAME="${RAND}_${RANDOM_FILE}"
  echo "Creating BMP file: $FULL_FILE_NAME"
  cp $DATASET_DIR/$RANDOM_FILE $OUTPUT_DIR/$FULL_FILE_NAME
done
