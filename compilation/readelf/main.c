#include <stdio.h>
#include <elf.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>

void print_help(char *binary)
{
  printf("Usage: %s [OPTION] [FILE]\n", binary);
  printf("Option:\n");
  printf("\t-h               - check if it is an ELF file\n");
  printf("\t-sec, --section  - print the name of all sections\n");
}

int check_is_elf(Elf64_Ehdr *elf, char **argv)
{
  // Check if it is an ELF file
  if (elf->e_ident[EI_MAG0] == ELFMAG0
      && elf->e_ident[EI_MAG1] == ELFMAG1
      && elf->e_ident[EI_MAG2] == ELFMAG2
      && elf->e_ident[EI_MAG3] == ELFMAG3)
    {
      printf("ELF file\n");
      return 0;
    }
  else
    {
      printf("%s: Error: %s is not an ELF file - Bad magical bytes\n", argv[0], argv[1]);
      printf("%s: Bytes: %d %d %d %d\n", argv[0], elf->e_ident[EI_MAG0], elf->e_ident[EI_MAG1], elf->e_ident[EI_MAG2], elf->e_ident[EI_MAG3]);
      return 3;
    }
}

int main(int argc, char **argv)
{
  // Return of program
  int ret_main = 0;
  
  // Check argument
  if (argc != 3)
    return print_help(argv[0]), 1;

  // Open binary file
  FILE *fd = fopen(argv[2], "r");
  if (!fd)
    {
      perror("open");
      return 2;
    }

  // Collect stat
  struct stat sta;
  stat(argv[2], &sta);

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

  if (strcmp(argv[1], "-h") == 0)
    {
      ret_main = check_is_elf(elf, argv);
    }
  else if (strcmp(argv[1], "-sec") == 0 || strcmp(argv[1], "--section") == 0)
    {
      ret_main = check_is_elf(elf, argv);

      if (ret_main)
        goto end_main;

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
    }
  else if (strcmp(argv[1], "-sym") == 0 || strcmp(argv[1], "--symbol") == 0)
    {
      ret_main = check_is_elf(elf, argv);

      if (ret_main)
        goto end_main;

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

          // Search symbole section
          int i = 0;
          for (i = 0; i < len; i++)
            {
              if (shdr[i].sh_type == SHT_SYMTAB)
                break;
            }

          // Check if symbol section is found
          if (i >= len)
            {
              printf("No symbol section\n");
              ret_main = 5;
              goto end_main;
            }

          // Symbol section found
          len = shdr[i].sh_size / shdr[i].sh_entsize;
          Elf64_Sym *sym = (Elf64_Sym *)((char *)elf + shdr[i].sh_offset);

          printf("Symbol:\n");
          printf("  %d symbol(s)\n", len);
          printf("  [Nr] %20s\n", "Name of symbol");

          // Check if table are not NULL
          // cast in char to move by bytes
          char *strtab = (char *)elf + shdr[i].sh_addr;
          if (!strtab)
            {
              printf("Error: strtab is NULL\n");
              ret_main = 4;
              goto end_main;
            }

          for (int i = 0; i < len; i++)
            {
              char *buff = strtab + sym[i].st_name;
              printf("  [%2d] %20s\n", i, buff);
            }
        }
    }
  else
    {
      print_help(argv[0]);
    }
  
  // End main label
 end_main:

  // Close binary file
  fclose(fd);
  
  //
  return ret_main;
}
