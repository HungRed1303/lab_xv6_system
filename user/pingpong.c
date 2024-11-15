#include "kernel/types.h"
#include "user/user.h"

int main()
{
	int p1[2]; //Pipe 1: Cha ghi p1[1], con doc p1[0]
	int p2[2]; //Pipe 2: Con ghi p2[1], cha doc p2[0]
	char message='A'; //Thong diep la byte 'A'
			 
	//Tao hai pipes
	pipe(p1); //Pipe cho cha -> con
	pipe(p2); //Pipe cho con -> cha
		 
	int pid=fork();//Tao tien trinh con
	
	if(pid>0)//dang trong tien trinh cha
	{
		//Cha ghi byte 'A' vao p1 (pipe1)
		write(p1[1],&message,1);
		close(p1[1]); //Dong dau ghi sau khi da ghi xong
		//Cho doi tien trinh con ket thuc
		wait(0);

		//Cha doc byte tu con qua p2(pipe2)
		read(p2[0],&message,1);

		printf("%d: received pong\n", getpid());

		//Dong dau doc sau khi doc xong
		close(p2[0]);
		close(p1[0]);
		close(p2[1]);
	}
	else if(pid==0)//dang trong tien trinh con
	{
		//Con doc byte tu p1(pipe1)
		read(p1[0],&message,1);
		printf("%d: received ping\n", getpid());

		//Ghi byte tra lai cho cha qua p2(pipe2)
		write(p2[1],&message,1);

		//Dong file descriptor
		close(p1[0]);
		close(p2[1]);
		close(p1[1]);
		close(p2[0]);

		//Tien trinh con ket thuc
		exit(0);
	}
	else//Khong tao duoc tien trinh con
	{
		printf("Fork failed.\n");
		exit(1);
	}
	//Tien trinh cha ket thuc
	exit(0);
}


