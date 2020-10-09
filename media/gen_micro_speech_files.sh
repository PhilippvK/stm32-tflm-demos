#!/bin/bash

DATASET_DIR=$1
OUTPUT_DIR=$2
COMMANDS=$3
COUNT=$4
SOX=$(which sox)

function print_usage() {
  echo 'Usage: ./gen_micro_speech_files.sh [DATASET_DIR] [OUTPUT_DIR] [COMMANDS] [COUNT]'
}

if [[ -z $SOX ]]
then
  echo "Program 'sox' is not installed! Aborting..."
  exit 1
fi

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
if [[ -z $COMMANDS ]]
then
  echo "COMMANDS not defined! Aborting..."
  print_usage
  exit 1
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

AVAILABLE_COMMANDS=`find $DATASET_DIR -type d -exec basename {} \; | grep -vE 'speech_dataset|_background_noise_'`
CHOSEN_COMMANDS=(`echo $COMMANDS | tr , "\n"`)

for COMMAND in ${CHOSEN_COMMANDS[@]}
do
  echo $AVAILABLE_COMMANDS | grep -wq $COMMAND
  if [[ $? -ne 0 ]]
  then
    echo "Command '${COMMAND}' not found! Aborting..."
    echo -e "Available commands:\n${AVAILABLE_COMMANDS}"
    exit 1
  fi
  COMMAND_FILES=(`find $DATASET_DIR/$COMMAND -iname '*.wav'`)
  #echo -e "COMMAND_FILES=\n${COMMAND_FILES[@]}"
  #echo
  for i in $(seq $COUNT)
  do
    RAND=`expr $RANDOM % 100`
    RANDOM_FILE=${COMMAND_FILES[$RAND % ${#COMMAND_FILES[@]}]}
    FILE_NAME=`echo $RANDOM_FILE | awk -F/ '{print $(NF-1)"_"$NF}'`
    FULL_FILE_NAME="${RAND}_${FILE_NAME}"
    echo "Creating WAV file: $FULL_FILE_NAME"
    $SOX $RANDOM_FILE $OUTPUT_DIR/$FULL_FILE_NAME channels 2
  done
done

