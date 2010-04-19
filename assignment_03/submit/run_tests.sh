#!/bin/sh
#
# First, put your input images in ./test_input, for example ./test_input/Duck_on_Pond.pgm , ./test_input/Snowy_Forest.pgm
#       (name them ending in ".pgm")
#
# Then, run this script.  It will:
#	1) Loop over each of your test images,
#	2) Run the imEnhance program on each one,
#	3)   at several window sizes (3x3, 5x5, 7x7, 9x9, etc.),
#	4)     at several num_threads values (1, 2, 4, etc.),
#	5) Place the output under ./test_output according to window size

imEnhance="./timEnhance"

INPUT_DIR="./test_input"
OUTPUT_DIR="./test_output"

time_cmd="/usr/bin/time"

mkdir $OUTPUT_DIR
if [ $? -ne 0 ]; then
	exit
fi;

echo "real, user, sys, window_size, num_threads, image_name, exit_status" > $OUTPUT_DIR/gnu_time.txt

for num_threads in 1 2 4 6; do	# num_threads
	for w in 3 5 7 9; do	# window_size

		mkdir -p "${OUTPUT_DIR}/${num_threads}_threads/${w}x${w}"

		for i in $INPUT_DIR/*.pgm; do
			i=`basename $i`

			TIME="%e ${num_threads}"
			export TIME

			$time_cmd -a -o $OUTPUT_DIR/gnu_time.txt  $imEnhance		\
				$INPUT_DIR/$i						\
				$OUTPUT_DIR/${num_threads}_threads/${w}x${w}/$i.avg.pgm			\
				$OUTPUT_DIR/${num_threads}_threads/${w}x${w}/$i.var.pgm			\
				$OUTPUT_DIR/${num_threads}_threads/${w}x${w}/$i.med.pgm			\
				$OUTPUT_DIR/${num_threads}_threads/${w}x${w}/$i.enh.pgm			\
				${w}							\
				${num_threads}
		done
	done
done

