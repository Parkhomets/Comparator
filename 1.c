#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void clrscr(void) {
  printf("\033[2J");
  printf("\033[0;0f");
}


char *optarg; /*ломается, если переношу в main*/


int main(int argc, char* argv[])
{
  FILE *f1, *f2, *f3;
  int N = 0, nums1, nums2, opt, optind, opterr, optopt, stop;
  long length1, length2, length;
  char *pattern, *example, *file3, choice;
  float step = 0;
  clrscr();
  if (argc != 9 && argc != 2)
  {
    printf("Incorrect number of arguments\n");
    printf("Use -h for more information\n");
    return(1);
  }

  while ((opt = getopt(argc, argv, "f:s:o:n:h")) != -1)
  {
    switch(opt)
    {
      case 'f':
        f1 = fopen(optarg, "rb");
        break;
      case 's':
        f2 = fopen(optarg, "rb");
        break;
      case 'o':
        file3 = optarg;
        f3 = fopen(optarg, "rb");
        break;
      case 'n':
        N = atoi(optarg);
        break;
      case 'h':
        printf("Comparator - программа для побайтового сравнения двух файлов.\n");
        printf("Использование:\n");
        printf("./Comparator -f [file1] -s [file2] -o [file3] -n [length], где\n");
        printf("file1 - проверочный файл\n");
        printf("file2 - проверяемый_файл\n");
        printf("file3 - файл_записи_результата\n");
        printf("length - длина_проверяемого_участка\n");
        printf("Пример:\n");
        printf("./Comparator -f example1.txt -s example2.txt -o log.txt -n 40\n");
        printf("./Comparator -n 100 -f example1.txt -s example2.txt -o log.txt\n");
        return(1);
        break;
    }
  }

  if (!f1){
    printf("The first file not found\n");
    return(1);
  }
  if (!f2){
    printf("The second file not found\n");
    return(1);
  }
  if (f3){
    printf("The third file is found\n");
    printf("Do you want to remove it? [Y/n]\n");
    scanf("%c", &choice);
    if (!(choice == 'n' || choice == 'N'))
    {
        remove(file3);
        f3 = fopen(file3, "w");
    }
    else
    {
        clrscr();
        printf("Use another file3\n");
        return(1);
    }
  }
  else
  {
    f3 = fopen(file3, "w");
  }
  if (N <= 0)
  {
    clrscr();
    printf("Вы не указали длину!\n");
    return(1);
  }

  pattern = (char*)malloc(N * sizeof(char));
  example = (char*)malloc(N * sizeof(char));
  fseek(f1, 0, SEEK_END);
  fseek(f2, 0, SEEK_END);
  length = (ftell(f1) < ftell(f2)) ? ftell(f1) : ftell(f2);
  rewind(f1);
  rewind(f2);

  while (1)
  {
    nums1 = fread(pattern, sizeof(char), N, f1); /*зачем тогда начальный вектор?*/
    nums2 = fread(example, sizeof(char), N, f2);
    int flag = 0;
    if (nums1 == 0 || nums2 == 0)
      break;
    clrscr();
    stop = 0;

    while (stop < nums1 && stop < nums2)
    {
      if (pattern[stop] != example[stop])
        flag = 1;
      stop++;
    }
    nums1 = (nums1 < nums2) ? nums1 : nums2;
    if (flag)
      fprintf(f3, "[%.lf:%.lf]\tare different\n", step, step+nums1);
    else
      fprintf(f3, "[%.lf:%.lf]\tare equal\n", step, step+nums1);
    step += nums1;
    printf("[+]Progress...%.lf%%\n", step / length * 100);
  }
  fclose(f1);
  fclose(f2);
  fclose(f3);
  return (0);
}
