#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

volatile uint shared=0;
lock_t* lk;
struct add {
	int a;
	int b;
};
void test(void* ab){
	int i;
	lock_acquire(lk);
	int temp;
	for(i=0;i<4000000;i++){
		temp = shared;
		temp = temp+1;
		shared=temp;
	}
	lock_release(lk);
	exit();
}
//int thread_create(void (*start_routine)(void*), void *arg);
int
main(int argc, char *argv[])
{
	lk = (lock_t*)malloc(sizeof(lock_t));
	lock_init(lk);
//	void* m1;
//	m1 = malloc(4096);
//	unsigned char *p = (unsigned char *)&test;
//	printf(1,"user data - stack = %d, fn pointer = %x\n  ",(int*)m1,p);
	struct add var;
	var.a = 10;
	var.b =20;
	thread_create(&test,(void*)&var);
	thread_create(&test,(void*)&var);
	thread_create(&test,(void*)&var);
	thread_create(&test,(void*)&var);
	thread_create(&test,(void*)&var);
	thread_create(&test,(void*)&var);
//	test(&var);
//	test(&var);
	thread_join();
	thread_join();
	thread_join();
	thread_join();
	thread_join();
	thread_join();


	printf(1,"shread = %d\n",shared);
	//return 0;
	exit();
}
