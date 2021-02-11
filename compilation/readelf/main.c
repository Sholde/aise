#include <stdio.h>
#include <elf.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>

int main(int argc, char **argv)
{
  // Return of program
  int ret_main = 0;
  
  // Check argument
  if (argc != 2)
    return printf("Usage: %s [FILE]\n", argv[0]), 1;

  // Open binary file
  FILE *fd = fopen(argv[1], "r");
  if (!fd)
    {
      perror("open");
      return 2;
    }

  // Collect stat
  struct stat sta;
  stat(argv[1], &sta);

  // Take file descriptor
  int fildes = fileno(fd);

  // Map file
  void *p = mmap(NULL, sta.st_size, PROT_READ, MAP_FILE | MAP_PRIVATE, fildes, 0);
  if (p == MAP_FAILED)
    {
      perror("mmap");
      ret_main = 2;
      goto end_main;
    }

  // Copy on elf variable
  Elf64_Ehdr *elf = (Elf64_Ehdr *)p;

  // Handle information
  if (elf->e_ident[EI_MAG0] == ELFMAG0
      && elf->e_ident[EI_MAG1] == ELFMAG1
      && elf->e_ident[EI_MAG2] == ELFMAG2
      && elf->e_ident[EI_MAG3] == ELFMAG3)
    {
      printf("ELF file\n");
    }
  else
    {
      printf("%s: Error: %s is not an ELF file - Bad magical bytes\n", argv[0], argv[1]);
      printf("%s: Bytes: %d %d %d %d\n", argv[0], elf->e_ident[EI_MAG0], elf->e_ident[EI_MAG1], elf->e_ident[EI_MAG2], elf->e_ident[EI_MAG3]);
      ret_main = 3;
      goto end_main;
    }

  // It is an ELF file

  // Check if setction header exist
  if (!elf->e_shnum)
    {
      printf("  No section header\n");
    }
  else
    {
      // Section information
      int len = elf->e_shnum;
      printf("Section:\n");
      printf("  %d section(s)\n", len);
      printf("  [Nr] %20s\n", "Name of section");

      // Section variable
      // Cast in char to move by bytes
      Elf64_Shdr *shdr = (Elf64_Shdr *)((char *)elf + elf->e_shoff);

      // Check if section is defined
      if (elf->e_shstrndx == SHN_UNDEF)
        {
          printf("Error: elf->e_shstrndx == SHN_UNDEF\n");
          ret_main = 4;
          goto end_main;
        }

      // Check if table are not NULL
      // Cast in char to move by bytes
      char *strtab = (char *)elf + shdr[elf->e_shstrndx].sh_offset;
      if (!strtab)
        {
          printf("Error: strtab is NULL\n");
          ret_main = 4;
          goto end_main;
        }

      // Print section information
      for (int i = 0; i < len; i++)
        {
          char *buff = strtab + shdr[i].sh_name;

          if (!buff)
            {
              printf("Error: elf->e_shstrndx == SHN_UNDEF\n");
              ret_main = 4;
              goto end_main;
            }

          printf("  [%2d] %20s\n", i, buff);
        }
    }
  
  // End main label
 end_main:

  // Close binary file
  fclose(fd);
  
  //
  return ret_main;
}
