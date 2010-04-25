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
#	5) Place the output under ./test_output according to window size and num_threads

 imEnhance="./imEnhance"
pimEnhance="./pimEnhance"
timEnhance="./timEnhance"

INPUT_DIR="./test_input"
OUTPUT_DIR="./test_output"

time_cmd="/usr/bin/time"

mkdir $OUTPUT_DIR
if [ $? -ne 0 ]; then
	echo "FATAL: ${OUTPUT_DIR} already exists!"
	exit -1
fi;


for i in $INPUT_DIR/*.pgm; do
	for w in 3 5 7 9; do	# window_size
		i=`basename $i`

		## SINGLE-THREADED		
		mkdir -p "${OUTPUT_DIR}/single_threaded/${w}x${w}"
		TIME="'${i}', '${w}', '%e'"; export TIME
		$time_cmd -a -o $OUTPUT_DIR/single_threaded.time  $imEnhance	\
			$INPUT_DIR/$i						\
			$OUTPUT_DIR/${num_threads}_threads/${w}x${w}/$i.avg.pgm	\
			$OUTPUT_DIR/${num_threads}_threads/${w}x${w}/$i.var.pgm	\
			$OUTPUT_DIR/${num_threads}_threads/${w}x${w}/$i.med.pgm	\
			$OUTPUT_DIR/${num_threads}_threads/${w}x${w}/$i.enh.pgm	\
			${w}

		for num_threads in 1 2 4 6; do	# num_threads

			## THREADS		
			mkdir -p "${OUTPUT_DIR}/${num_threads}_threads/${w}x${w}"
			TIME="'${i}', '${w}', '${num_threads}', '%e'"; export TIME
			$time_cmd -a -o $OUTPUT_DIR/${num_threads}_threads.time $timEnhance	\
				$INPUT_DIR/$i							\
				$OUTPUT_DIR/${num_threads}_threads/${w}x${w}/$i.avg.pgm		\
				$OUTPUT_DIR/${num_threads}_threads/${w}x${w}/$i.var.pgm		\
				$OUTPUT_DIR/${num_threads}_threads/${w}x${w}/$i.med.pgm		\
				$OUTPUT_DIR/${num_threads}_threads/${w}x${w}/$i.enh.pgm		\
				${w}								\
				${num_threads}

			## PROCESSES
			mkdir -p "${OUTPUT_DIR}/${num_threads}_processes/${w}x${w}"
			TIME="'${i}', '${w}', '${num_threads}', '%e'"; export TIME
			$time_cmd -a -o $OUTPUT_DIR/${num_threads}_processes.time $pimEnhance	\
				$INPUT_DIR/$i							\
				$OUTPUT_DIR/${num_threads}_processes/${w}x${w}/$i.avg.pgm	\
				$OUTPUT_DIR/${num_threads}_processes/${w}x${w}/$i.var.pgm	\
				$OUTPUT_DIR/${num_threads}_processes/${w}x${w}/$i.med.pgm	\
				$OUTPUT_DIR/${num_threads}_processes/${w}x${w}/$i.enh.pgm	\
				${w}								\
				${num_threads}
		done
	done
done

exit 0
