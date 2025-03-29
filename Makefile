set_spinlock:
	mkdir -p build
	gcc set/spinlock_set.c evaluate_set.c -o build/set_spinlock -Iinclude -DSPINLOCK_VERSION -g

stack_cas:
	mkdir -p build
	gcc stack/cas_stack.c evaluate_stack.c -o build/stack_cas -Iinclude -CAS_VERSION -g

stack_spinlock:
	mkdir -p build
	gcc stack/spinlock_stack.c evaluate_stack.c -o build/stack_spinlock -Iinclude -DSPINLOCK_VERSION -g

stack_mutex:
	mkdir -p build
	gcc stack/mutex_stack.c evaluate_stack.c -o build/stack_mutex -Iinclude -DMUTEX_VERSION -g

clean:
	rm build/