#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

pthread_cond_t cond1;
pthread_mutex_t lock1;
bool ready = false;

void* provide(void* arg)
{
	while (1) 
	{
		pthread_mutex_lock(&lock1);
		
		if (ready == true)
		{
			pthread_mutex_unlock(&lock1);
			continue;
		}
	
		ready = true;
		cout << "Message provided" << endl;
		pthread_cond_signal(&cond1);
		pthread_mutex_unlock(&lock1);
		
		sleep(1);
	}
	
	return nullptr;
}

void* consume(void* arg)
{
	while (1)
	{
		pthread_mutex_lock(&lock1);
		
		while (ready == false)
		{
			pthread_cond_wait(&cond1, &lock1);
		}
		
		ready = false;
		cout << "Message consumed" << endl;
		pthread_mutex_unlock(&lock1);
	}

	return nullptr;
}

int main()
{
	pthread_t threads[2];

	pthread_cond_init(&cond1, nullptr);
	pthread_mutex_init(&lock1, nullptr);

	pthread_create(&threads[0], nullptr, provide, nullptr);
	pthread_create(&threads[1], nullptr, consume, nullptr);

	pthread_join(threads[0], nullptr);
	pthread_join(threads[1], nullptr);

	pthread_mutex_destroy(&lock1);
	pthread_cond_destroy(&cond1);
}
