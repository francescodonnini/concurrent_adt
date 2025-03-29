set_mutex:
	mkdir -p build
	gcc set/set_mutex.c evaluate_set.c -o build/set_mutex -Iinclude -DMUTEX_VERSION -g

set_spinlock:
	mkdir -p build
	gcc set/set_spinlock.c evaluate_set.c -o build/set_spinlock -Iinclude -DSPINLOCK_VERSION -g

stack_cas:
	mkdir -p build
	gcc stack/stack_cas.c evaluate_stack.c -o build/stack_cas -Iinclude -g

stack_mutex:
	mkdir -p build
	gcc stack/stack_mutex.c evaluate_stack.c -o build/stack_mutex -Iinclude -DMUTEX_VERSION -g

stack_spinlock:
	mkdir -p build
	gcc stack/stack_spinlock.c evaluate_stack.c -o build/stack_spinlock -Iinclude -DSPINLOCK_VERSION -g

clean:
	rm build/