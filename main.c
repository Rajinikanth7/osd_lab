//Pipe.c LAB-4 SOLUTION
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
void quit (char *message, int exit_status)
{
 printf(" %s",message);
 exit(exit_status);
}
int main()
{
 int fd[2]; /* To be filled up by pipe() */
 if (pipe(fd < 0) /* Now have four descriptors for pipe */
 printf("os lead");
 switch (fork())
 {
 case -1: quit("fork", 2);
 case 0: close(fd[0]); /* CHILD - Close read end first*/
 dup2(fd[1], STDOUT_FILENO); /*Connect stdout to write end */
 close(fd[1]); /* and close original descriptor */
 execlp("cat", "cat", "os.txt", (char *) 0);
 quit("cat", 3);
 default:close(fd[1]); /*PARENT - Close write end first */
 dup2(fd[0], STDIN_FILENO); /*Connect stdin to read end*/
 close(fd[0]); /* and close original descriptor */
 execlp("tr", "tr", "[a-z]","[A-Z]", (char *) 0);
 quit("tr", 4);
 }
}






//unlink.c LAB-4 SOLUTION
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
int main(argc, argv)
int argc;
char *argv[];
{
int fd;
char buf[10241];
struct stat statbuf;
if(argc !=2) /* need a parameter */
exit(1);
fd = open(argv[1], O_RDONLY);
if(fd ==-1) /* open fails */
exit(1);
if(unlink(argv[1])==-1) /* unlink file just opened */
exit(1);
if(stat(argv[1], &statbuf)==-1) /* stat the file by name*/
printf("stat %s fails as it should\n", argv[1]);
else
printf("stat %s succeeded!!!!\n", argv[1]);
if(fstat(fd, &statbuf)==-1)
{ /* stat the file by fd */
printf("fstat %s fails!!!\n", argv[1]);
}
else
{
printf("fstat %s succeeds as it should\n", argv[1]);
}
while(read(fd,buf,sizeof(buf)>0)) /* read open/unlinked file */
{
printf("%1024s", buf); /* prints 1K byte field */
}
}




//attributes.c LAB-5 SOLUTION
attributes.c
#include <stdio.h>
#include <sys/stat.h> /* For struct stat */
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
void quit(char *message, int exit_status)
{
 printf(" %s",message);
 exit(exit_status);
}
void arg_check (long int args, long int argc, char *message, long int exit_status)
{
if (argc != args)
{
printf("%s", message);
exit(exit_status);
}
}
int main(long int argc, char **argv)
{
struct stat statbuf; /* We’ll use lstat to populate this */
arg_check(2, argc, "Single filename required\n", 1) ;
if(lstat(argv[1], &statbuf) == -1)
quit("Couldn’t stat file", 1);
printf("File: %s\n", argv[1]);
printf("Inode number: %ld \n", statbuf.st_ino);
printf("UID: %ld ", statbuf.st_uid);
printf("GID: %ld\n", statbuf.st_gid);
printf("Type and Permissions: %o\n",statbuf.st_mode);
printf("Number of links: %ld \n", statbuf.st_nlink);
printf("Size in bytes: %ld\n", statbuf.st_size);
printf("Blocks allocated: %ld\n", statbuf.st_blocks);
printf("Last Modification Time: %s", ctime(&statbuf.st_mtime));
printf("Last Access Time: %s\n", ctime(&statbuf.st_atime));
exit(0);
}





//lsdir.c LAB-5 SOLUTION
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
void quit (char *message, int exit_status)
{
 printf(" %s",message);
 exit(exit_status);
}
void arg_check(int args, int argc, char *message, int exit_status)
{
if(argc != args)
{
printf("%s", message);
exit(exit_status);
}
}
int main(int argc, char *argv[])
{
DIR *dir;
struct dirent *direntry; /* Returned by readdir() */
struct stat statbuf; /* Address of statbuf used by lstat() */
mode_t file_type,file_perm;
arg_check(2, argc, "Directory not specified\n", 1) ;
if((dir = opendir(argv[1])) == NULL)
quit("Couldn’t open directory", 1);
if((chdir(argv[1]) == -1)) /* Change to the directory before */
quit("chdir", 2); /* you starting reading its entries */
while((direntry = readdir(dir)) != NULL)
{
/* Read each entry in directory*/
if(lstat(direntry->d_name, &statbuf) < 0)
{
/* dname must be in */
perror("lstat");
/* current directory */
continue;
}
if(S_ISDIR(statbuf.st_mode))
{
/* If file is a directory */
file_type = statbuf.st_mode & S_IFMT;
file_perm = statbuf.st_mode & ~S_IFMT;
printf("%o %4o %s\n",file_type,file_perm,direntry->d_name);
}
}
exit(0);
}





//signal.c: Catching the SIGALRM  LAB-6 SOLUTION
#include <stdio.h>
#include <sys/stat.h> /* For struct stat */
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#define BUFSIZE 100
void alrm_handler(int signo); /* Prototype declaration */
char buf[BUFSIZE] = “foo\0”; /* Global variable */
void quit(char *message, int exit_status)
{
 printf(" %s",message);
 exit(exit_status);
}
int main (void) {
int n;
struct sigaction act;
act.sa_handler = alrm_handler; /* Specify handler */
if (sigaction(SIGALRM, &act, NULL) == -1) /* Install handler */
quit(“sigalrm”, 1);
fprintf(stderr, “Enter filename: “);
alarm(5); /* Set alarm clock; will deliver */
n = read(STDIN_FILENO, buf, BUFSIZE); /* SIGALRM in 5 seconds */
if (n > 1) /* Will come here if user inputs */
fprintf(stderr, “Filename: %s\n”, buf); /* string within 5 seconds */
exit(0);
}




//LAB-6 SOLUTIONS
//Program: killprocess.c -- Uses fork and exec to run a user-defined program
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
pid_t pid;
int main (int argc, char **argv) {
int i, status;
void death_handler(int signo); /* A common signal handler this time */
struct sigaction act;
act.sa_handler = death_handler;
sigaction(SIGCHLD, &act, NULL); /* Disposition for these two signals */
sigaction(SIGALRM, &act, NULL); /* set to enter a single handler */
switch (pid = fork()) {
case -1: fprintf(stderr, “Fork error\n”);
case 0: execvp(argv[1], &argv[1]); /* Execute command */
perror(“exec”);
break;
default: alarm(5); /* Will send SIGALRM after 5 seconds */
pause(); /* Will return when SIGCHLD signal is received */
fprintf(stderr, “Parent dies\n”);
}
exit(0);
}
void death_handler(int signo) { /* This common handler picks up the */
int status; /* exit status for normal termination */
/* but sends the SIGTERM signal if */
switch (signo) { /* command doesn’t complete in 5 seconds */
case SIGCHLD: waitpid(-1, &status, 0); /* Same as wait(&status); */
fprintf(stderr, “Child dies; exit status: %d\n”,
WEXITSTATUS(status));
break;
case SIGALRM: if (kill(pid, SIGTERM) == 0)
fprintf(stderr, “5 seconds over, child killed\n”);
}
}
void alrm_handler(int signo)
{ 
fprintf(stderr, “\nSignal %d received, default filename: %s\n”, signo, buf);
exit(1);
}





//program to demonstrate time and times System Call.
//Times.c:  LAB-6 SOLUTION
#include <stdio.h>
#include <unistd.h>
#include <sys/times.h>
#include <time.h>
int main() {
 struct tms times_start, times_end;
 clock_t times_start_retval, times_end_retval;
 clock_t clock_start, clock_end;
 int i;
 /* clock called first and last, so estimates using "clock" should be
 slightly longer than estimates using "times" */
 if((clock_start = clock()) == -1) {
 perror("starting clock");
 return -1;
 }
 if((times_start_retval = times(&times_start)) == -1) {
 perror("starting times");
 return -1;
 }
 for(i = 100000000; i; i--); // do work
 if((times_end_retval = times(&times_end)) == -1) {
 perror("ending timer");
 return -1;
 }
 printf("Times using the clock system call\n");
 printf("clock start: %li\nclock end: %li\n", clock_start, clock_end);
 printf("elapsed: %li\n\n", clock_end - clock_start);
 printf("Times using the times system call\n");
 printf("System start: %li\nUser start: %li, start retval: %li\n",
 times_start.tms_stime, times_start.tms_utime, times_start_retval);
 printf("System end: %li\nUser end: %li, end retval: %li\n",
 times_end.tms_stime, times_end.tms_utime, times_end_retval);
 printf("elapsed: %li\n\n", times_end_retval - times_start_retval);
 return 0;
}




//Time.c: LAB-6
/* time example */
#include <stdio.h> /* printf */
#include <time.h> /* time_t, struct tm, difftime, time, mktime */
int main ()
{
 time_t timer;
 struct tm y2k = {0};
 double seconds;
 y2k.tm_hour = 0; y2k.tm_min = 0; y2k.tm_sec = 0;
 y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;
 time(&timer); /* get current time; same as: timer = time(NULL) */
 seconds = difftime(timer,mktime(&y2k));
 printf ("%.f seconds since January 1, 2000 in the current timezone",
seconds);
 return 0;
}





// Segment.C:LAB-7 SOLUTION
#include<stdio.h>
#include<malloc.h>
int glb_uninit; /* Part of BSS Segment -- global uninitialized variable, at
 runtime it is initialized to zero */
int glb_init = 10; /* Part of DATA Segment -- global initialized variable */
void foo(void)
{
static int num = 0; /* stack frame count */
int autovar; /* automatic variable/Local variable */
int *ptr_foo = (int*)malloc(sizeof(int)); /* allocate memory using malloc*/
if (++num == 4) /* Creating four stack frames */
return;
printf("Stack frame number %d: address of autovar: %p\n", num, & autovar);
printf("Address of heap allocated inside foo() %p\n",ptr_foo);
foo();/* function call */
}
int main()
{
char *p, *b, *nb;
int *ptr_main = (int*)malloc(sizeof(int)); /* allocate memory using malloc*/
printf("Text Segment:\n");
printf("Address of main: %p\n", main);
printf("Address of afunc: %p\n",foo);
printf("Stack Locations:\n");
foo();
printf("Data Segment:\n");
printf("Address of glb_init: %p\n", & glb_init);
printf("BSS Segment:\n");
printf("Address of glb_uninit: %p\n", & glb_uninit);
printf("Heap Segment:\n");
printf("Address of heap allocated inside main() %p\n",ptr_main);
return 0;
}




//Free and sbrk.c: LAB-7 SOLUTIONS
#define MAX_ALLOCS 1000000
#include <stdio.h> /* Standard I/O functions */
#include <stdlib.h> /* Prototypes of commonly used library
functions,plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h> /* Prototypes for many system calls */
#include <errno.h> /* Declares errno and defines error constants */
#include <string.h> /* Commonly used string-handling functions */
int main(int argc, char *argv[])
{
char *ptr[MAX_ALLOCS];
int freeStep, freeMin, freeMax, blockSize, numAllocs,j;
printf("\n");
if (argc < 3 || strcmp(argv[1], "--help") == 0)
{
printf("%s num-allocs block-size [step [min [max]]]\n"
argv[0]);
exit(5);
}
numAllocs = strtol(argv[1], NULL, 10);
if (numAllocs > MAX_ALLOCS)
{ 
printf("num-allocs > %d\n", MAX_ALLOCS);
exit(5);
}
blockSize = strtol(argv[2], NULL, 10);
freeStep = (argc > 3) ? strtol(argv[3], NULL, 10): 1;
freeMin = (argc > 4) ? strtol(argv[4], NULL, 10) : 1;
freeMax = (argc > 5) ? strtol(argv[5], NULL, 10) : numAllocs;
if (freeMax > numAllocs)
{
printf("free-max > num-allocs\n");
exit(5);
}
printf("Initial program break: %10p\n", sbrk(0));
printf("Allocating %d*%d bytes\n", numAllocs, blockSize);
for (j = 0; j < numAllocs; j++)
{
ptr[j] = malloc(blockSize);
if (ptr[j] == NULL)
{
perror("malloc");
exit(5);
}
}
printf("Program break is now: %10p\n", sbrk(0));
printf("Freeing blocks from %d to %d in steps of
%d\n",freeMin, freeMax, freeStep);
for (j = freeMin -1;
j < freeMax;
j += freeStep)free(ptr[j]);
printf("After free(), program break is: %10p\n",
sbrk(0));
exit(10);
}



//Malloc.c: lAB -7 SOLUTIONS

#include <sys/types.h> /* Type definitions used by many programs */
#include <stdio.h> /* Standard I/O functions */
#include <stdlib.h> /* Prototypes of commonly used library functions,plus
EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h> /* Prototypes for many system calls */
#include <errno.h> /* Declares errno and defines error constants */
#include <string.h> /* Commonly used string-handling functions */
extern char end;
void *my_malloc (size_t);
void my_free(void *);
struct blk {size_t size;
struct blk *prev;
struct blk *next;};
struct blk *first = NULL;
struct blk *last = NULL;
void *my_malloc (size_t size) {size_t required_size = size + sizeof(struct
blk);
struct blk *curr = first;
while (curr != NULL && curr->size < required_size)
{
curr = curr->next;
}
if (curr == NULL) {void *new = sbrk((intptr_t) required_size);
if (new == (void *) -1) { return NULL; }
struct blk *new_blk = (struct blk *) new;
new_blk->size = required_size;
return (void *) (new_blk + 1);}
if (curr == first) { first = first->next; }
else { curr->prev->next = curr->next; }
if (curr == last) { last = last->prev; }
else {curr->next->prev = curr->prev;
}if (curr->size > required_size + sizeof(struct blk)) {struct blk *left =
(struct blk *) (((char *) curr) + required_size);
left->size = curr->size -required_size;
curr->size = required_size;
my_free((char *) (left + 1));}
return (void *) (curr + 1);}
void my_free (void *ptr) {struct blk *blk_ptr = ((struct blk *) ptr) -1;
if (first == NULL) {first = last = blk_ptr;return;}
if (blk_ptr < first) {blk_ptr->prev = NULL;
if (((char *) blk_ptr) + blk_ptr->size == (char *) first) {blk_ptr->size +=
first->size;
blk_ptr->next = first->next;}
else {first->prev = blk_ptr;blk_ptr->next = first;}first =
blk_ptr;return;}
if (blk_ptr > last) {if (((char *) last) + last->size == (char *) blk_ptr)
{last->size += blk_ptr->size;}
else {blk_ptr->next = NULL;
blk_ptr->prev = last;
last->next = blk_ptr;
last = blk_ptr;}
return;}
struct blk *curr = first;
while (curr < blk_ptr) {curr = curr->next;}
struct blk *before = curr->prev;
if (((char *) before) + before->size == (char *) blk_ptr) {before->size +=
blk_ptr->size;
blk_ptr = before;}
else {blk_ptr->prev = before;
before->next = blk_ptr;}
if (((char *) blk_ptr) + blk_ptr->size == (char *) curr) {blk_ptr->size +=
curr->size;
blk_ptr->next = curr->next;
curr->next->prev = blk_ptr;
}
else {blk_ptr->next = curr;
curr->prev = blk_ptr;}}
#define MAX_ALLOCS 1000000
int main (int argc, char *argv[]) {
/* copied from free_and_sbrk.c --licensed by Michael Kerrisk under the
GPLv3 */
char *ptr[MAX_ALLOCS];
int freeStep, freeMin, freeMax, blockSize, numAllocs, j;
printf("\n");
if (argc < 3 || strcmp(argv[1], "--help") == 0) {printf("%s num-allocs
block-size [step [min [max]]]\n", argv[0]);
perror("num-allocs block-size");}
numAllocs = strtol(argv[1], NULL, 10);
if (numAllocs > MAX_ALLOCS) {printf("num-allocs > %d\n", MAX_ALLOCS);
perror("num-allocs");}
blockSize = strtol(argv[2], NULL, 10);
freeStep = (argc > 3) ? strtol(argv[3], NULL, 10) : 1;
freeMin = (argc > 4) ? strtol(argv[4], NULL, 10) : 1;
freeMax = (argc > 5) ? strtol(argv[5], NULL, 10): numAllocs;
if (freeMax > numAllocs) {perror("free-max > num-allocs");}
printf("Initial program break: %10p\n", sbrk(0));
printf("Allocating %d*%d bytes\n", numAllocs, blockSize);
for (j = 0; j < numAllocs; j++) {
ptr[j] = my_malloc(blockSize);
if (ptr[j] == NULL) {perror("malloc");}
printf("%10p\n", sbrk(0));}
printf("Program break is now: %10p\n", sbrk(0));
printf("Freeing blocks from %d to %d in steps of %d\n",freeMin, freeMax,
freeStep);
for (j = freeMin -1; j < freeMax; j += freeStep) {my_free(ptr[j]);}
printf("After my_free(), program break is: %10p\n", sbrk(0));
exit(EXIT_SUCCESS);
    
}




//Write a program to demonstrate Accessing Memory with Paging - linear translates.
//Pagetable.c: LAB -8 SOLUTION
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
struct page{
int page_no;
int frame;
};
int main()
{
int size_logical_address,size_physical_address,i,size_of_page,j;
printf("Enter size of logical address space: ");
scanf("%d",&size_logical_address);
printf("Enter size of physical address space: ");
scanf("%d",&size_physical_address);
printf("Enter size of page: ");
scanf("%d",&size_of_page);
int number_of_frames = size_physical_address/size_of_page;
int number_of_pages = size_logical_address/size_of_page;
struct page pageTable[number_of_pages];
printf("Enter page table: \n");
for(i=0;i<number_of_pages;i++)
{
pageTable[i].frame = -1;
}
for(i=0;i<number_of_pages;i++)
{
int frame;
bool replica = false;
pageTable[i].page_no = i;
printf("Enter frame for %d page number(-1 if frame doesn't exist): ",i);
scanf("%d",&frame);
for(j=0;j<number_of_pages;j++)
{
if(frame!= -1 && pageTable[j].frame == frame)
{
replica = true;
printf("Frame number already stored\n");
}
}
if(frame > number_of_frames)
{
replica = true;
printf("Cannot exceed frame size\n");
}
if(replica == false)
{
pageTable[i].frame = frame;
}
}
int logical_address;
printf("Enter -1 to exit\n");
while(1)
{
printf("Enter logical address: ");
scanf("%d",&logical_address);
if(logical_address == -1)
return 0;
int page_no = logical_address/size_of_page;
int offset = logical_address%size_of_page;
if(pageTable[page_no].frame == -1)
{
printf("No such logical address exist\n");
}
else
{
printf("Page no: %d \nOffset: %d\nFrame no: %d\nPhysical address:
%d\n",page_no, offset, pageTable[page_no].frame,pageTable[page_no].frame*size_of_page +
offset );
}
}
}



//Write a program that translates logical to physical addresses for a virtual address space of size
//2^16 = 65,536 bytes. Your program will read from a file containing logical addresses and, using a
//TLB as well as a page table
//Virtual_mem.c: LAB-8 SOLUTION
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#define PAGE_SIZE 256
#define PAGE_ENTRIES 256
#define PAGE_NUM_BITS 8
#define FRAME_SIZE 256
#define FRAME_ENTRIES 256
#define MEM_SIZE (FRAME_SIZE * FRAME_ENTRIES)
#define TLB_ENTRIES 16
int
virtual,page_number,offset,physical,frame_number,value,page_table[PAGE_ENTRIES],tlb[TLB_E
NTRIES][2],tlb_front = -1,tlb_back = -1,mem_index = 0,fault_counter = 0,tlb_counter = 0;
char memory[MEM_SIZE];
int address_counter = 0;
float fault_rate,tlb_rate;
int get_physical(int virtual),get_offset(int virtual);
int get_page_number(int virtual);
void initialize_page_table(int n);
void initialize_tlb(int n);
int consult_page_table(int page_number);
int consult_tlb(int page_number);
void update_tlb(int page_number, int frame_number);
int get_frame();
int main(int argc, char *argv[]) {

 char* in_file;
 char* out_file;
 char* store_file;
 char* store_data;
 int store_fd;
 char line[8];
 FILE* in_ptr;
 FILE* out_ptr;
 initialize_page_table(-1);
 initialize_tlb(-1);
 if (argc != 4) {
 printf("Enter input, output, and store file names!");
 exit(EXIT_FAILURE);
 }
 else {
 in_file = argv[1];
 out_file = argv[2];
 store_file = argv[3];
 if ((in_ptr = fopen(in_file, "r")) == NULL) {
 printf("Input file could not be opened.\n");
 exit(EXIT_FAILURE);
 }
 if ((out_ptr = fopen(out_file, "a")) == NULL) {
 printf("Output file could not be opened.\n");
 exit(EXIT_FAILURE);
 }
 store_fd = open(store_file, O_RDONLY);
 store_data = mmap(0, MEM_SIZE, PROT_READ, MAP_SHARED, store_fd, 0);
 if (store_data == MAP_FAILED) {
 close(store_fd);
 printf("Error mmapping the backing store file!");
 exit(EXIT_FAILURE);
 }
 while (fgets(line, sizeof(line), in_ptr)) {
 virtual = atoi(line);
 address_counter++;
 page_number = get_page_number(virtual);
 offset = get_offset(virtual);
 frame_number = consult_tlb(page_number);
 if (frame_number != -1) {
 physical = frame_number + offset;
 value = memory[physical];
 }
 else {
 frame_number = consult_page_table(page_number);
 if (frame_number != -1) {
 physical = frame_number + offset;
 update_tlb(page_number, frame_number);
 value = memory[physical];
 }
 else {
 int page_address = page_number * PAGE_SIZE;
 if (mem_index != -1) {
 memcpy(memory + mem_index,
 store_data + page_address, PAGE_SIZE);
 frame_number = mem_index;
 physical = frame_number + offset;

 value = memory[physical];

 page_table[page_number] = mem_index;

 update_tlb(page_number, frame_number);

 if (mem_index < MEM_SIZE - FRAME_SIZE) {
 mem_index += FRAME_SIZE;
 }
 else {

 mem_index = -1;
 }
 }
 else {

 }
 }
 }
 fprintf(out_ptr, "Virtual address: %d ", virtual);
 fprintf(out_ptr, "Physical address: %d ", physical);
 fprintf(out_ptr, "Value: %d\n", value);
 }
 fault_rate = (float) fault_counter / (float) address_counter;
 tlb_rate = (float) tlb_counter / (float) address_counter;
 fprintf(out_ptr, "Number of Translated Addresses = %d\n", address_counter);
 fprintf(out_ptr, "Page Faults = %d\n", fault_counter);
 fprintf(out_ptr, "Page Fault Rate = %.3f\n", fault_rate);
 fprintf(out_ptr, "TLB Hits = %d\n", tlb_counter);
 fprintf(out_ptr, "TLB Hit Rate = %.3f\n", tlb_rate);
 fclose(in_ptr);
 fclose(out_ptr);
 close(store_fd);
 }
 return EXIT_SUCCESS;
}
int get_physical(int virtual) {
 physical = get_page_number(virtual) + get_offset(virtual);
 return physical;
}
int get_page_number(int virtual) {

 return (virtual >> PAGE_NUM_BITS);
}
int get_offset(int virtual) {

 int mask = 255;
 return virtual & mask;
}
void initialize_page_table(int n) {
 for (int i = 0; i < PAGE_ENTRIES; i++) {
 page_table[i] = n;
 }
}
void initialize_tlb(int n) {
 for (int i = 0; i < TLB_ENTRIES; i++) {
 tlb[i][0] = -1;
 tlb[i][1] = -1;
 }
}
int consult_page_table(int page_number) {
 if (page_table[page_number] == -1) {
 fault_counter++;
 }
 return page_table[page_number];
}
int consult_tlb(int page_number) {
 for (int i = 0; i < TLB_ENTRIES; i++) {
 if (tlb[i][0] == page_number) {
 tlb_counter++;
 return tlb[i][1];
 }
 }

 return -1;
}
void update_tlb(int page_number, int frame_number) {

 if (tlb_front == -1) {

 tlb_front = 0;
 tlb_back = 0;
 
 tlb[tlb_back][0] = page_number;
 tlb[tlb_back][1] = frame_number;
 }
 else {

 tlb_front = (tlb_front + 1) % TLB_ENTRIES;
 tlb_back = (tlb_back + 1) % TLB_ENTRIES;

 tlb[tlb_back][0] = page_number;
 tlb[tlb_back][1] = frame_number;
 }
 return;
}




//Write a program for the simulation of following paging algorithms FIFO LRU and
//MRU NFU.
//FIFO: LAB-9 SOLUTIONS
/*
FIFO Page Replacement Algorithm
*/
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
int pointer;
int faults ,hits;
void print(int frame_size,int frame[])
{
int i;
//printf("Printing the Frames: ");
for(i=0;i<frame_size;i++)
{
if(frame[i]==-1)
printf("- ");
else
printf("%d ",frame[i]);
}
printf("\n");
}
void add_reference(int frame_size,int frame[], int reference)
{
int i;
bool alloted = false;
for(i=0;i<frame_size;i++)
{
if(frame[i]==reference)
{
alloted = true;
printf(" Hit for %d | ", reference);
hits++;
break;
}
else if(frame[i]==-1)
{
alloted = true;
frame[i] = reference;
printf("Fault for %d | ", reference);
faults++;
break;
}
}
if(alloted == false)
{
faults++;
printf("Fault for %d | ", reference);
frame[pointer] = reference;
pointer = (pointer+1)%frame_size;
}
print(frame_size, frame);
}
int main()
{
int frame_size,i,number_of_references;
printf("Enter frame size: ");
scanf("%d",&frame_size);
int frame[frame_size];
for(i=0;i<frame_size;i++)
{
frame[i] = -1;
}
print(frame_size,frame);
printf("Enter the number of references: ");
scanf("%d",&number_of_references);
int reference[number_of_references];
for(i=0;i<number_of_references;i++)
{
scanf("%d",&reference[i]);
add_reference(frame_size,frame,reference[i]);
}
printf("\nNumber of faults: %d \nNumber of hits: %d\n",faults,hits );
}


//LRU: LAB-9 SOLUTIONS
/*
Least Recently Used Page Replacement Algorithm
*/
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
int pointer;
int faults ,hits;
void print(int frame_size,int frame[])
{
int i;
//printf("Printing the Frames: ");
for(i=0;i<frame_size;i++)
{
if(frame[i]==-1)
printf("- ");
else
printf("%d ",frame[i]);
}
printf("\n");
}
int predict(int reference_length, int references[], int page_no ,int frame_size,int frame[], int start)
{
int pos = -1, farthest = start, i;
for(i=0;i<frame_size;i++)
{
int j;
for(j=start-1;j>=0;j--)
{
if(frame[i]==references[j])
{
if(j<farthest)
{
farthest=j;
pos=i;
}
break;
}
}
if(j==page_no)
return i;
}
if(pos == -1)
return 0;
else
return pos;
}
void add_reference(int frame_size,int frame[], int reference, int current_position,int reference_length,
int references[])
{
int i;
bool allocated=false;
for(i=0;i<frame_size;i++)
{
if(frame[i]==reference)
{
printf(" Hit for %d | ", reference);
hits++;
allocated = true;
break;
}
else if(frame[i]==-1)
{
frame[i] = reference;
printf("Fault for %d | ", reference);
faults++;
allocated = true;
break;
}
}
if(allocated==false)
{
int j =
predict(reference_length,references,current_position,frame_size,frame,current_position+1);
frame[j] = reference;
printf("Fault for %d | ", reference);
faults++;
}
print(frame_size, frame);
}
int main()
{
int frame_size,i,number_of_references;
printf("Enter frame size: ");
scanf("%d",&frame_size);
int frame[frame_size];
for(i=0;i<frame_size;i++)
{
frame[i] = -1;
}
print(frame_size,frame);
printf("Enter the number of references: ");
scanf("%d",&number_of_references);
int reference[number_of_references];
for(i=0;i<number_of_references;i++)
{
scanf("%d",&reference[i]);
add_reference(frame_size,frame,reference[i],i,number_of_references,reference);
}
printf("\nNumber of faults: %d \nNumber of hits: %d\n",faults,hits );
}




//Shm.c: LAB-10 SOLUTION
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define SHM_SIZE 1024 /* make it a 1K shared memory segment */
int main(int argc, char *argv[])
{
 key_t key;
 int shmid;
 char *data;
 int mode;
 if (argc > 2) {
 fprintf(stderr,"usage: shmdemo [data_to_write]\n");
 exit(1);
 }
/* make the key: */
if ((key = ftok("shm.c", 'R')) == -1)
{
 perror("ftok");
 exit(1);
}
/* connect to (and possibly create) the segment: */
if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1)
{
 perror("shmget");
 exit(1);
}
/* attach to the segment to get a pointer to it:*/
data = shmat(shmid, (void *)0, 0);
if (data == (char *)(-1))
{ perror("shmat");
 exit(1);
}
/* read or modify the segment, based on the command line: */
if (argc == 2)
{
 printf("writing to segment: \"%s\"\n", argv[1]);
 strncpy(data, argv[1], SHM_SIZE);
} else
{
 printf("segment contains: \"%s\"\n", data);
/* detach from the segment: */
 shmctl(shmid, IPC_RMID, NULL);
}
if (shmdt(data) == -1) {
 perror("shmdt");
 exit(1);
}
}




//Pthread.c: LAB-10 SOLUTIONS
#include <stdio.h>
#include <pthread.h>
void* worker( void* p ) {
 int* ip = (int*)p;
 printf("Hello world from thread ID:%d,thread number %i!\n",pthread_self(),*ip);
}
int main() {
 pthread_t OtherThread[4];
 int i;
 for(i=0;i<4;i++) {
 pthread_create( &OtherThread[i], NULL, worker, &i );
 sleep(1);
 }
}




//Pre-Lab: LAB-11 SOLUTIONS
//Main-signal.c:
#include <stdio.h>
#include <pthread.h>
int done = 0;
void* worker(void* arg) {
 printf("this should print first\n");
 done = 1;
 return NULL;
}
int main(int argc, char *argv[]) {
 pthread_t p;
 pthread_create(&p, NULL, worker, NULL);
 while (done == 0)
;
 printf("this should print last\n");
 return 0;
}





//Main-signal-cv.c:LAB-11 SOLUTIONS
#include <stdio.h>
#include <pthread.h>
// simple synchronizer: allows one thread to wait for another structure
"synchronizer_t" has all the needed data methods are:
// init (called by one thread)
// wait (to wait for a thread)
// done (to indicate thread is done)
typedef struct __synchronizer_t {
 pthread_mutex_t lock;
 pthread_cond_t cond;
 int done;
} synchronizer_t;
synchronizer_t s;
void signal_init(synchronizer_t *s) {
 pthread_mutex_init(&s->lock, NULL);
 pthread_cond_init(&s->cond, NULL);
 s->done = 0;
}
void signal_done(synchronizer_t *s) {
 pthread_mutex_lock(&s->lock);
 s->done = 1;
 pthread_cond_signal(&s->cond);
 pthread_mutex_unlock(&s->lock);
}
void signal_wait(synchronizer_t *s) {
 pthread_mutex_lock(&s->lock);
 while (s->done == 0)
pthread_cond_wait(&s->cond, &s->lock);
 pthread_mutex_unlock(&s->lock);
}
void* worker(void* arg) {
 printf("this should print first\n");
 signal_done(&s);
 return NULL;
}
int main(int argc, char *argv[]) {
 pthread_t p;
 signal_init(&s);
 pthread_create(&p, NULL, worker, NULL);
 signal_wait(&s);
 printf("this should print last\n");
 return 0;
}




//1. Illustrate how mutex is used for thread synchronization, print the counter variable upon
//each increment which is in the critical section. (Two threads update a global shared
//variable with and without synchronization).-LAB-11 SOLUTION
#include <pthread.h>
static volatile int glob = 0; /* "volatile" prevents compiler
optimizations
 of arithmetic operations on
'glob' */
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static void * /* Loop 'arg' times incrementing
'glob' */
threadFunc(void *arg)
{
 int loops = *((int *) arg);
 int loc, j;
 for (j = 0; j < loops; j++) {
 pthread_mutex_lock(&mtx);
loc = glob;
 loc++;
 glob = loc;
pthread_mutex_unlock(&mtx);
 }
 return NULL;
}
int
main(int argc, char *argv[])
{
 pthread_t t1, t2;
 int loops, s;
 loops = strtol(argv[1], NULL, 10);
 s = pthread_create(&t1, NULL, threadFunc, &loops);
 if (s != 0){
 perror("pthread_create fail");
exit(1);}
 s = pthread_create(&t2, NULL, threadFunc, &loops);
 if (s != 0){
 perror("pthread_create fail");
exit(2);}
 s = pthread_join(t1, NULL);
 if (s != 0){
 perror("pthread_join fail");
exit(1);}
 s = pthread_join(t2, NULL);
 if (s != 0){
 perror("pthread_join fail");
exit(1);}
 printf("glob = %d\n", glob);
 exit(0);
}




//2. Write a UNIX system program to implement concurrent Linked List--LAB-11 SOLUTION
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
typedef struct __node_t {
 int key;
 struct __node_t *next;
} node_t;
// basic list structure (one used per list)
typedef struct __list_t {
node_t *head;
pthread_mutex_t lock;
} list_t;
void List_Init(list_t *L) {
L->head = NULL;
pthread_mutex_init(&L->lock, NULL);
}
void List_Insert(list_t *L, int key) {
// synchronization not needed
node_t *new = malloc(sizeof(node_t));
if (new == NULL) {
perror("malloc");
return;
}
new->key = key;
// just lock critical section
pthread_mutex_lock(&L->lock);
new->next = L->head;
L->head = new;
pthread_mutex_unlock(&L->lock);
}
int List_Lookup(list_t *L, int key) {
int rv = -1;
pthread_mutex_lock(&L->lock);
node_t *curr = L->head;
while (curr) {
if (curr->key == key) {
rv = 0;
break;
}
curr = curr->next;
}
pthread_mutex_unlock(&L->lock);
return rv; // now both success and failure
}
void List_Print(list_t *L) {
 node_t *tmp = L->head;
 while (tmp) {
printf("%d ", tmp->key);
tmp = tmp->next;
 }
 printf("\n");
}
int main(int argc, char *argv[])
{
 list_t mylist;
 List_Init(&mylist);
 List_Insert(&mylist, 10);
 List_Insert(&mylist, 30);
 List_Insert(&mylist, 5);
 List_Print(&mylist);
 printf("In List: 10? %d 20? %d\n",
 List_Lookup(&mylist, 10), List_Lookup(&mylist, 20));
 return 0;
}



//Write a Unix System program to make A Parent Waiting for Its Child using semaphores.--LAB-11 SOLUTION
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
sem_t done;
void *
child(void *arg) {
 sleep(5);
 printf("child\n");
 sem_post(&done);
 return NULL;
}
int
main(int argc, char *argv[]) {
 pthread_t p;
 printf("parent: begin\n");
 sem_init(&done,0, 0);
 pthread_create(&p, NULL, child, NULL);
 sem_wait(&done);
 printf("parent: end\n");
 return 0;
}

