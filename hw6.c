#include <stdio.h>			//The long list of inputs that i tried, and some i needed
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>



void printDirectory(DIR* dp);		//voids PrintDirectroy
void extractData(pid_t pid);		//voids out extractData


/*
*
*The main function which takes a proces specified by the user or no process at all and prints out all running processes.
*@param int argc used to identy how many parameters are passed into main, char*argv[] is user input
*
*/
int main(int argc, char*argv[])
{	
 if(argc!=2)
	{
	char* argv[] = {"/proc"};
	const char* direc = argv[0];
	DIR *dp = opendir(direc);
	printDirectory(dp);
	}
	else
	{
	char path[30];
	char pathEx[30];
	
	strcpy(pathEx,argv[1]);
	strcpy(path,"/proc/");

	strcat(path, pathEx);
	
	pid_t pid = (pid_t) atoi (pathEx);	//if specific pid is entered by user then convert it to actual pid
	
	extractData(pid);			//use the extractData(pid) with the pid from above to print out only one process.
	}

	return 0;
}

/*
*
*The printDirectory(DIR* dp) prints out all the information for every directory in /proc
*@param dp is a directory that is specified.
*
*/
void printDirectory(DIR* dp)
{
	struct dirent *dir;
	struct stat *st;
	if(dp==NULL)
	{
		fprintf(stderr, "cannot open dir, you idiot\n");
		return;
	}
	while((dir=readdir(dp))!=NULL)
	{
		if((dir->d_ino !=0) && (strcmp(dir->d_name, "..")!= 0) && (strcmp (dir->d_name, ".") != 0))
		{
			if(dir->d_type == DT_DIR)
			{
			//printf("The process ID is %s\n",dir->d_name);
			//printf("The group ID is %d\n",st->st_gid);
	
			pid_t pid = (pid_t) atoi (dir->d_name);	
			extractData(pid);
			printf("\n");	
			}	
		}
	}
	closedir(dp);
}

/*
*
*
*The extractData(pid_t pid) function takes a pid and adds it to the path for the stat file. It then uses sscanf to determine the data in the *stat file. Then I print out the data that we need from the stat file. The information we needed was process id, process group id, state, the *proces priority, memory size, and set size.
*
*
*/
void extractData(pid_t pid)
{

	int fd;
	char filename[24];
	char arg_list[1024];
	size_t length;
	char* next_arg;

	char comm[24];
	char state[24];
	int ppid;
	int pgid;
	int session;
	int tty_nr;
	int tpgid;
	unsigned int flags;
	unsigned long minflt;
	unsigned long cminflt;
	unsigned long majflt;
	unsigned long cmajflt;
	unsigned long utime;
	unsigned long stime;
	long int cutime;
	long int cstime;
	long int priority;
	long int nice;
	long int num_threads;
	long int itrealvalue;
	unsigned long long starttime;
	unsigned long vsize;
	long int rss;


	snprintf(filename, sizeof(filename), "/proc/%d/stat", (int) pid);

	fd = open(filename, O_RDONLY); 
	length = read(fd, arg_list, sizeof(arg_list));
	close(fd);

	arg_list[length] = '\0';

	next_arg = arg_list;

		while(next_arg < arg_list + length)
		{
		sscanf(next_arg, "%d %s %s %d %d %d %d %d %d %ld %ld %ld %lu %lu %lu %ld %ld %ld %ld %ld %ld %llu %lu %ld ", &pid, comm, 	state, &ppid, &pgid, &session, &tty_nr, &tpgid, &flags, &minflt, &cminflt, &majflt, &cmajflt, &utime, &stime, &cutime, &cstime, &priority, &nice, &num_threads, &itrealvalue, &starttime, &vsize, &rss);

		printf("Process ID:%d\n", pid);
		printf("Process name of executable:%s: \n", comm);
		printf("Process State:%s\n", state);
		printf("Process Group ID:%d\n", pgid);
		printf("Process Priority:%ld\n", priority);
		printf("Virtual memory size:%lu\n", vsize);
		printf("Resident Set Size:%ld\n", rss);
		next_arg += strlen (next_arg) + 1;
		}


   


}




