project14: mainFileReading.o instructionDecoding.o passUtil.o first_pass.o second_pass.o base64Converter.o output_files.o
	gcc -g -Wall -ansi -pedantic mainFileReading.o instructionDecoding.o passUtil.o first_pass.o second_pass.o output_files.o base64Converter.o -o project14

mainFileReading.o: mainFileReading.c dataStructures.h output_files.h
	gcc -g -Wall -ansi -pedantic -c mainFileReading.c -o mainFileReading.o

first_pass.o: first_pass.c first_pass.h dataStructures.h passUtil.h
	gcc -g -Wall -ansi -pedantic -c first_pass.c -o first_pass.o

second_pass.o: second_pass.c second_pass.h dataStructures.h passUtil.h
	gcc -g -Wall -ansi -pedantic -c second_pass.c -o second_pass.o

instructionDecoding.o: instructionDecoding.c instructionDecoding.h dataStructures.h
	gcc -g -Wall -ansi -pedantic -c instructionDecoding.c -o instructionDecoding.o

passUtil.o: passUtil.c passUtil.h dataStructures.h
	gcc -g -Wall -ansi -pedantic -c passUtil.c -o passUtil.o

base64Converter.o: base64Converter.c base64Converter.h
	gcc -g -Wall -ansi -pedantic -c base64Converter.c -o base64Converter.o

output_files.o: output_files.c output_files.h dataStructures.h base64Converter.h
	gcc -g -Wall -ansi -pedantic -c output_files.c -o output_files.o

clean:
	-rm -f *.o *.ob *.ext *.ent

