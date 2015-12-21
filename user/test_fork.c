#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

struct add {
	int a;
	int b;
};
long shared=0;
lock_t lk;

//simple test
void test(void* ab){
	struct add *var = (struct add *)ab;
	printf(1,"val recieved a = %d b =%d\n",var->a,var->b);
	exit();
}

//inc for race
void inc(void* ab){
	printf(1,"---in inc---\n");
	long i=0;int tmp;int j;
	for(i=0;i<1000;i++){
		lock_acquire(&lk);
		tmp=shared;
		for(j=0;j<50;j++)
		shared=tmp+1;
		lock_release(&lk);
	}
	exit();
}

//test recursion , thread stack overflow if val >~ 1020
void recur1(int val){
	printf(1,"in recur\n");
	if(val==0) return;
	recur1(val-1);
}

void recursive(void * val){
	int *num =(int *)val;
	recur1(*num);
	exit();
}


//one thread , multi join calls
void multiJoin(){
	 struct add var;
	  var.a = 10;
	  var.b =20;
	  thread_create(&test,(void*)&var);
	  wait();
//	  thread_join();
//	  thread_join();
//	  thread_join();
}

//call clone , join raw
void testcloneRaw(){
	struct add var;
	var.a = 10;
	var.b =20;
	void* stack= malloc(4096);
	clone(&test,(void*)&var,stack);

	void *stack1 = malloc(sizeof(void*));
	int result= join(&stack1);
	printf(1,"restult from join %d",result);
	free(stack1);

	void *stack2 = malloc(sizeof(void*));
	int result1= join(&stack2);
	printf(1,"restult from join %d",result1);
	free(stack2);
}



//two children , each child 30 threads
void testChildThread(){
	int pid1 = fork();
	if(pid1==0){
		int i=0;
		lock_init(&lk);
		for(i=0;i<10;i++)
		for(i=0;i<10;i++)
		thread_join();
		printf(1,"shared=%d",shared);
		exit();
	}else{
		int pid2=fork();
		if(pid2==0){
				int i=0;
				lock_init(&lk);
				for(i=0;i<10;i++)
				thread_create(&inc,(void*)NULL);
				for(i=0;i<10;i++)
				thread_join();
				printf(1,"shared=%d",shared);
				exit();
			}else{
				wait();
			}
		wait();
	}
}

//thread calls recursive func
void resurseTest(){
	int val = 1000;
	thread_create(&recursive,(void*)&val);
	thread_join();
}

//thread create then wait()
void createWait(){
	thread_create(&inc,(void*)NULL);
	thread_create(&inc,(void*)NULL);
	thread_create(&inc,(void*)NULL);
	thread_create(&inc,(void*)NULL);
}
int
main(int argc, char *argv[])
{
	createWait();
	resurseTest();
	multiJoin();
	testChildThread();
	testcloneRaw();
	exit();
}
