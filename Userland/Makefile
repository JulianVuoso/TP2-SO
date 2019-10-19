include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

all: library sampleCodeModule sampleDataModule processA processB shell

library:
	cd Library; make

sampleCodeModule:
	cd SampleCodeModule; make

processA:
	cd ProcessA; make

processB:
	cd ProcessB; make

shell:
	cd Shell; make

sampleDataModule:
	printf "This is sample data." >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	cd Library; make clean
	cd SampleCodeModule; make clean
	cd ProcessA; make clean
	cd ProcessB; make clean
	cd Shell; make clean
	rm -rf *.bin


.PHONY: library sampleCodeModule processA processB shell all clean
