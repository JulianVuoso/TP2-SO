# TPOueS
Trabajo Practico destinado para la materia de Arquitectura de Computadoras.

GDB DEBUGGER

-S 		no inicia CPU
-s		gdb.tcp:1234 (gdbserver en 1234)
-monitor dev	abre una terminal para interactuar con qemu
-d int		logging interrupciones

-Le vamos a decir a qemu que arranque sin iniciar CPU, para poder abrir el debugger. Nos vamos a conectar a qemu desde gdb para poder controlar desde ahi la ejecucion de la maquina qemu.

sudo ./docker.sh jtallar gdb

-Para obtener la ip
ifconfig | grep "status: active" -B4 | grep "inet " | xargs | cut -d " " -f2 | pbcopy

-Inicio gdb con: gdb
target remote ... (direccion ip)

- Como el archivo de salida (en los .ld) es binary, se tira toda la info de debuggeo (aunque compile con GCC con -G). En gdb le podemos decir que los simbolos estan en kernel.lf y 0000-sampleCodeModule.elf.

add-symbol-file kernel.elf 0x100000
add-symbol-file	0000-sampleCodeModule.elf 0x400000

- b get_char 	agrega breakpoint en la direccion de inicio de la funcion get_char
- c		continuar hasta llegar a un breakpoin
- n		avanzar de a una linea
- si		avanza de a una linea por assembler
- ENTER		ejecuta el ultimo comando

-Si corremos gdb con un .gdbinit, se pone lindo. (en el .gdbinit, habra que cambiar la direccion ip)

-En la CheatSheet, están los comandos más importantes

-En el manual de qemu, en la sección de gdb, hay un par de comandos para habilitar las interrupciones nuevamente (por defecto, me las deshabilita)

-Si agregamos -d int en el run.sh, imprime por salida estandar las llamadas a interrupciones, registros, segmentos, etc.

- Para mostrar solo las lineas en las que se ejecutaron interrupciones (se podria elegir un solo tipo en el "")
	./run.sh 2>&1 | grep "v=[0-9]\+"

	Sirve por ejemplo para ver que llamadas como get_char bloqueen, no llamen constantemente (busy waiting)

-----------------------------------------------------------------------------------------------------
MEMORY MANAGER

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


