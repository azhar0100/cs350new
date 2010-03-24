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

#imEnhance="./imEnhance.gprof.o"
imEnhance="./imEnhance"

mkdir $OUTPUT_DIR
if [ $? -ne 0 ]; then
	exit
fi;

for n in 3 5 7 9 15; do

	mkdir "${OUTPUT_DIR}/${n}x${n}"

	for i in $INPUT_DIR/*.pgm; do
		i=`basename $i`
		$imEnhance					\
			$INPUT_DIR/$i				\
			$OUTPUT_DIR/${n}x${n}/$i.mean.pgm	\
			$OUTPUT_DIR/${n}x${n}/$i.var.pgm	\
			$OUTPUT_DIR/${n}x${n}/$i.median.pgm	\
			$OUTPUT_DIR/${n}x${n}/$i.enhanced.pgm	\
			${n}
	done
done

