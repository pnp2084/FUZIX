/*
 *	Generate the syscall functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syscall_name.h"

static char namebuf[128];

static void write_call(int n)
{
  FILE *fp;
  int saves = 0;
  snprintf(namebuf, 128, "fuzixtms9900/syscall_%s.S", syscall_name[n]);
  fp = fopen(namebuf, "w");
  if (fp == NULL) {
    perror(namebuf);
    exit(1);
  }
  fprintf(fp, "\tpseg\n\n"
	      "\teven\n"
	      "\tdef %s\n"
	      "%s\n", syscall_name[n], syscall_name[n]);
  fprintf(fp, "\txop @%d,1\n", n);
  fprintf(fp, "\tjoc 1f\n");
  fprintf(fp, "\tmov r0,r2\n");
  fprintf(fp, "\tb *r11\n");
  fprintf(fp, "1:\tmov r0,@errno\n");
  fprintf(fp, "\tli r2,-1\n");
  fprintf(fp, "\tb *r11\n");

  fclose(fp);
}

static void write_call_table(void)
{
  int i;
  for (i = 0; i < NR_SYSCALL; i++)
    write_call(i);
}

static void write_makefile(void)
{
  int i;
  FILE *fp = fopen("fuzixtms9900/Makefile", "w");
  if (fp == NULL) {
    perror("Makefile");
    exit(1);
  }
  fprintf(fp, "# Autogenerated by tools/syscall_tms9900\n");
  fprintf(fp, "CROSS_AS=tms9900-gcc\nCROSS_LD=tms9900-ld\nCROSS_AR=tms9900-ar\n");
  fprintf(fp, "ASOPTS=\n\n");
  fprintf(fp, "ASRCS = syscall_%s.S\n", syscall_name[0]);
  for (i = 1; i < NR_SYSCALL; i++)
    fprintf(fp, "ASRCS += syscall_%s.S\n", syscall_name[i]);
  fprintf(fp, "\n\nASRCALL = $(ASRCS) $(ASYS)\n");
  fprintf(fp, "\nAOBJS = $(ASRCALL:.S=.o)\n\n");
  fprintf(fp, "syslib.lib: $(AOBJS)\n");
  fprintf(fp, "\techo $(AOBJS) >syslib.l\n");
  fprintf(fp, "\t$(CROSS_AR) rc syslib.lib $(AOBJS)\n\n");
  fprintf(fp, "$(AOBJS): %%.o: %%.S\n");
  fprintf(fp, "\t$(CROSS_AS) $(ASOPTS) -c $<\n\n");
  fprintf(fp, "clean:\n");
  fprintf(fp, "\trm -f $(AOBJS) $(ASRCS) syslib.lib syslib.l *~\n\n");
  fclose(fp);
}

int main(int argc, char *argv[])
{
  write_makefile();
  write_call_table();
  exit(0);
}