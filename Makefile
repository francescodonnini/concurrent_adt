set_mutex:
	mkdir -p build
	gcc set/set_mutex.c evaluate_set.c -o build/set_mutex -Iinclude -DMUTEX_VERSION -g -lpthread

set_serial:
	mkdir -p build
	gcc set/set_serial.c evaluate_serial_set.c -o build/set_serial -Iinclude -g -lpthread

set_spinlock:
	mkdir -p build
	gcc set/set_spinlock.c evaluate_set.c -o build/set_spinlock -Iinclude -DSPINLOCK_VERSION -g -lpthread

stack_backoff:
	mkdir -p build
	gcc stack/stack_backoff.c evaluate_concurrent_stack.c -o build/stack_backoff -Iinclude -g -lm -lpthread -DBACKOFF_VERSION

stack_cas:
	mkdir -p build
	gcc stack/stack_cas.c evaluate_concurrent_stack.c -o build/stack_cas -Iinclude -g -lpthread

stack_mutex:
	mkdir -p build
	gcc stack/stack_mutex.c evaluate_concurrent_stack.c -o build/stack_mutex -Iinclude -DMUTEX_VERSION -g -lpthread

stack_serial:
	mkdir -p build
	gcc stack/stack_serial.c evaluate_serial_stack.c -o build/stack_serial -Iinclude -g -lpthread

stack_spinlock:
	mkdir -p build
	gcc stack/stack_spinlock.c evaluate_concurrent_stack.c -o build/stack_spinlock -Iinclude -DSPINLOCK_VERSION -D__USE_XOPEN2K -D_GNU_SOURCE -g -lpthread

clean:
	rm build/*