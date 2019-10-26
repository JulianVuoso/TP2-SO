include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

all: library sampleCodeModule sampleDataModule processA processB shell idle sleep loop cat wc filter phylo

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

idle:
	cd Idle; make

sleep:
	cd Sleep; make	

loop:
	cd Loop; make

cat:
	cd Cat; make	

wc:
	cd Wc; make	

filter:
	cd Filter; make

phylo:
	cd Phylo; make

sampleDataModule:
	printf "This is sample data." >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	cd Library; make clean
	cd SampleCodeModule; make clean
	cd ProcessA; make clean
	cd ProcessB; make clean
	cd Shell; make clean
	cd Idle; make clean
	cd Sleep; make clean
	cd Loop; make clean
	cd Cat; make clean
	cd Wc; make clean
	cd Filter; make clean
	cd Phylo; make clean
	rm -rf *.bin


.PHONY: library sampleCodeModule processA processB shell idle sleep loop cat wc filter phylo all clean
