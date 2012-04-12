#ifdef __APPLE__
	#include <mach/semaphore.h>
	#include <mach/mach.h>
	#include <mach/task.h>
#else
	#include <semaphore.h>
#endif

typedef void * universal_semaphore;

int	universal_sem_create(universal_semaphore semaphore, const int value);
int	universal_sem_wait(universal_semaphore semaphore);
int	universal_sem_signal(universal_semaphore semaphore);
int universal_sem_destroy(universal_semaphore semaphore);

int universal_sem_create(universal_semaphore semaphore, const int value) {
	int i;
	#ifdef __APPLE__
		i = semaphore_create(mach_task_self(), (semaphore_t *) semaphore, SYNC_POLICY_FIFO, value);
	#else
		i = sem_init((sem_t *) semaphore, 0, value);
	#endif
	return i;
}

int universal_sem_wait(universal_semaphore semaphore) {
	int i;
	#ifdef __APPLE__
		i = semaphore_wait(* (semaphore_t *) semaphore);
	#else
		i = sem_wait((sem_t *) semaphore);
	#endif
	return i;
}

int universal_sem_signal(universal_semaphore semaphore) {
	int i;
	#ifdef __APPLE__
		i = semaphore_signal(* (semaphore_t *) semaphore);
	#else
		i = sem_post((sem_t *) semaphore);
	#endif
	return i;
}

int universal_sem_destroy(universal_semaphore semaphore) {
	int i;
	#ifdef __APPLE__
		i = semaphore_destroy(mach_task_self(), * (semaphore_t *) semaphore);
	#else
		i = sem_destroy((sem_t *) semaphore);
	#endif
	return i;
}
