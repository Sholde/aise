#include <stdio.h>
#include <elf.h>

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

  // Read file
  Elf64_Ehdr elf;
  int ret_fread = fread(&elf, sizeof(Elf64_Ehdr), 1, fd);
  if (!ret_fread)
    {
      perror("fread");
      ret_main = 2;
      goto end_main;
    }

  // Handle information
  if (elf.e_ident[EI_MAG0] == ELFMAG0
      && elf.e_ident[EI_MAG1] == ELFMAG1
      && elf.e_ident[EI_MAG2] == ELFMAG2
      && elf.e_ident[EI_MAG3] == ELFMAG3)
    {
      printf("ELF file\n");
    }
  else
    {
      printf("%s: Error: %s is not an ELF file - Bad magical bytes\n", argv[0], argv[1]);
      printf("%s: Bytes: %d %d %d %d\n", argv[0], elf.e_ident[EI_MAG0], elf.e_ident[EI_MAG1], elf.e_ident[EI_MAG2], elf.e_ident[EI_MAG3]);
      ret_main = 3;
      goto end_main;
    }

  // It is an ELF file

  // Check if setction header exist
  if (!elf.e_shnum)
    {
      printf("  No section header\n");
    }
  else
    {
      // Section information
      int len = elf.e_shnum;
      printf("Section:\n");
      printf("  %d section(s)\n", len);
      printf("  [Nr]         Name\n");

      // Section variable
      Elf64_Shdr *shdr = (Elf64_Shdr *)(&elf + elf.e_shoff);

      // Check if section is defined
      if (elf.e_shstrndx == SHN_UNDEF)
        {
          printf("Error: elf.e_shstrndx == SHN_UNDEF\n");
          ret_main = 4;
          goto end_main;
        }

      // Check if table are not NULL
      char *strtab = (char *)&elf + shdr[elf.e_shstrndx].sh_offset;
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
              printf("Error: elf.e_shstrndx == SHN_UNDEF\n");
              ret_main = 4;
              goto end_main;
            }
          
          printf("  [%2d] %12s\n", i, buff);
        }
    }
  
  // End main label
 end_main:

  // Close binary file
  fclose(fd);
  
  //
  return ret_main;
}
