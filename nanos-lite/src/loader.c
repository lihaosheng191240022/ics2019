#include "proc.h"
#include <elf.h>

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

#include"fs.h"
extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
static uintptr_t loader(PCB *pcb, const char *filename) {
  /*pa3.2*/
	//ramdisk_read((void *)0x3000000, 0, 0x63f8);
	//ramdisk_read((void *)0x3008000, 0x7000, 0x8b8);	
  //return 0x30002e8;
/*
	Elf_Ehdr Elfheader;
	_my_debug_ printf("size of Elfheadr=%d\n", sizeof(Elf_Ehdr));
	ramdisk_read(&Elfheader, 0, 52);
	Elf_Phdr Progheader;
	size_t Phdrstart = Elfheader.e_phoff;
	size_t Phdrsize = Elfheader.e_phentsize;
	for(int i=0;i<Elfheader.e_phnum;i++){
		ramdisk_read(&Progheader, Phdrstart + i * Phdrsize, Phdrsize);
		if(Progheader.p_type==PT_LOAD){
			ramdisk_read((void *)Progheader.p_vaddr, Progheader.p_offset, Progheader.p_memsz);
		}
	}
	return Elfheader.e_entry;
	*/
	Elf_Ehdr elfheader;
	Elf_Phdr progheader;
	
	int fd = fs_open(filename, 0, 0);
	fs_lseek(fd, 0, SEEK_SET);
	fs_read(fd, &elfheader, sizeof(Elf_Ehdr));
	//fs_lseek(fd, elfheader.e_phoff, SEEK_SET);
	_my_debug_ printf("hello1\n");
	size_t entry_size = elfheader.e_phentsize;
	size_t entry_num = elfheader.e_phnum;
	_my_debug_ printf("entry_size = %u, entry_num = %u\n", entry_size, entry_num);
	for(size_t i=0;i<entry_num;i++){
		fs_lseek(fd, elfheader.e_phoff+i*entry_size, SEEK_SET);
		fs_read(fd, &progheader, entry_size);
		
		if(progheader.p_type==PT_LOAD){
			fs_lseek(fd, progheader.p_offset, SEEK_SET);
			fs_read(fd, (void *)progheader.p_vaddr, progheader.p_memsz);
		}
	}
	_my_debug_ printf("hello2\n");
	fs_close(fd);
	_my_debug_ printf("entry = %x\n", elfheader.e_entry);
	return elfheader.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
