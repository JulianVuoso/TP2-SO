# TPOueS
Trabajo Practico destinado para la materia de Arquitectura de Computadoras.

Para ver las distintas secciones de memoria
	readelf --sections /bin/ls

KERNEL.LD
	
	Veo desde donde arranca cada seccion de memoria ".text", las demas secciones estaran pegadas.

	Variables interesantes --> 	.text arranca en 0x100000
					endOfKernelBinary
					endOfKernel

sampleCodeModule.ld
	Variable interesante -->	endOfBinary

qemu <...> -monitor stdio 	--> Para abrir una terminal para interactuar con qemu


Analizar tamaño de SampleCodeModule, ver que no se solape con SampleDataModule
Verlo con endOfUserBinary

Analizar tamaño de SampleDataModule --> Haciendo un size de SampleDataModule
Asegurarnos que esa direccion no sobrepase la direccion inicial de memoria a administrar
Podemos tomar directamente a partir de una direccion como 0x600000

freeRTOS/Source/portable/MemMgr


