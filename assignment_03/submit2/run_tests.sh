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
	for w in 3 5 7; do  # window_size

		i=`basename $i`
		mkdir -p $OUTPUT_DIR/times/$i

#		## SINGLE-THREADED		
#		echo "Single-threaded, window_size = ${w}"
#		mkdir -p "${OUTPUT_DIR}/single_threaded/${w}x${w}"
#		TIME="%e"; export TIME
#		$time_cmd -a -o $OUTPUT_DIR/times/$i/single_threaded_${w}x${w}.gnuplot $imEnhance	\
#			$INPUT_DIR/$i									\
#			$OUTPUT_DIR/single_threaded/${w}x${w}/$i.avg.pgm				\
#			$OUTPUT_DIR/single_threaded/${w}x${w}/$i.var.pgm				\
#			$OUTPUT_DIR/single_threaded/${w}x${w}/$i.med.pgm				\
#			$OUTPUT_DIR/single_threaded/${w}x${w}/$i.enh.pgm				\
#			${w}
#		if [ $? -ne 0 ]; then
#			echo "FATAL: Last run returned non-zero status."
#			exit -1
#		fi

		for num_threads in 1 2 3 4 5 6 8 10 20 40 60 80 100; do  # num_threads

			## THREADS		
			echo "Threads: ${num_threads}, window_size = ${w}"
			mkdir -p "${OUTPUT_DIR}/${num_threads}_threads/${w}x${w}"
			TIME="${num_threads} %e"; export TIME
			$time_cmd -a -o $OUTPUT_DIR/times/$i/threaded_${w}x${w}.gnuplot $timEnhance	\
				$INPUT_DIR/$i								\
				$OUTPUT_DIR/${num_threads}_threads/${w}x${w}/$i.avg.pgm			\
				$OUTPUT_DIR/${num_threads}_threads/${w}x${w}/$i.var.pgm			\
				$OUTPUT_DIR/${num_threads}_threads/${w}x${w}/$i.med.pgm			\
				$OUTPUT_DIR/${num_threads}_threads/${w}x${w}/$i.enh.pgm			\
				${w}									\
				${num_threads}
			if [ $? -ne 0 ]; then
				echo "FATAL: Last run returned non-zero status."
				exit -1
			fi

			## PROCESSES
			echo "Processes: ${num_threads}, window_size = ${w}"
			mkdir -p "${OUTPUT_DIR}/${num_threads}_processes/${w}x${w}"
			TIME="${num_threads} %e"; export TIME
			$time_cmd -a -o $OUTPUT_DIR/times/$i/processes_${w}x${w}.gnuplot $pimEnhance	\
				$INPUT_DIR/$i								\
				$OUTPUT_DIR/${num_threads}_processes/${w}x${w}/$i.avg.pgm		\
				$OUTPUT_DIR/${num_threads}_processes/${w}x${w}/$i.var.pgm		\
				$OUTPUT_DIR/${num_threads}_processes/${w}x${w}/$i.med.pgm		\
				$OUTPUT_DIR/${num_threads}_processes/${w}x${w}/$i.enh.pgm		\
				${w}									\
				${num_threads}
			if [ $? -ne 0 ]; then
				echo "FATAL: Last run returned non-zero status."
				exit -1
			fi

		done
	done
done

exit 0
