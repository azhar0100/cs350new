#!/bin/sh
#
# First, put your input images in ./test_input, for example ./test_input/Duck_on_Pond.pgm , ./test_input/Snowy_Forest.pgm
#       (name them ending in ".pgm")
#
# Then, run this script.  It will:
#	1) Loop over each of your test images,
#	2) Run the imEnhance program on each one,
#	3)   at several window sizes (3x3, 5x5, 7x7, 9x9, etc.),
#	3) Place the output under ./test_output according to window size, thus:
#
#		./test_output/3x3/Duck_on_Pond.enhanced.pgm
#		./test_output/3x3/Duck_on_Pond.mean.pgm
#		./test_output/3x3/Duck_on_Pond.median.pgm
#		./test_output/3x3/Duck_on_Pond.var.pgm
#
#		./test_output/3x3/Snowy_Forest.enhanced.pgm
#		./test_output/3x3/Snowy_Forest.mean.pgm
#		./test_output/3x3/Snowy_Forest.median.pgm
#		./test_output/3x3/Snowy_Forest.var.pgm
#
#		./test_output/5x5/Duck_on_Pond.enhanced.pgm
#		./test_output/5x5/Duck_on_Pond.mean.pgm
#		./test_output/5x5/Duck_on_Pond.median.pgm
#		./test_output/5x5/Duck_on_Pond.var.pgm
#
#		./test_output/5x5/Snowy_Forest.enhanced.pgm
#		./test_output/5x5/Snowy_Forest.mean.pgm
#		./test_output/5x5/Snowy_Forest.median.pgm
#		./test_output/5x5/Snowy_Forest.var.pgm
#
#		etc. etc.
#
# If you wish to re-run the script, you will need to rm or mv ./test_output.

INPUT_DIR="./test_input"
OUTPUT_DIR="./test_output"

#imEnhance="./timEnhance.gprof.o"
imEnhance="./timEnhance"

time_cmd="/usr/bin/time"

echo "real, user, sys, window_size, num_threads, image_name, exit_status" > $OUTPUT_DIR/gnu_time.txt

mkdir $OUTPUT_DIR
if [ $? -ne 0 ]; then
	exit
fi;


for num_threads in 1 2 3 4 5 7 9 10 16; do	# num_threads
	for w in 3 7 15; do	# window_size

		mkdir "${OUTPUT_DIR}/${w}x${w}"

		for i in $INPUT_DIR/*.pgm; do
			i=`basename $i`

			TIME="%e, %U, %S, ${w}, ${num_threads}, ${i}, %x"
			export TIME

			$time_cmd -a -o $OUTPUT_DIR/gnu_time.txt  $imEnhance		\
				$INPUT_DIR/$i						\
				$OUTPUT_DIR/${w}x${w}/$i.avg.pgm			\
				$OUTPUT_DIR/${w}x${w}/$i.var.pgm			\
				$OUTPUT_DIR/${w}x${w}/$i.med.pgm			\
				$OUTPUT_DIR/${w}x${w}/$i.enh.pgm			\
				${w}							\
				${num_threads}
		done
	done
done

