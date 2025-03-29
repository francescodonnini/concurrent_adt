set_spinlock:
	gcc set/spinlock_set.c evaluate_set.c -o build/set_spinlock -Iinclude -DSPINLOCK_VERSION -g

stack_cas:
	gcc stack/cas_stack.c evaluate_stack.c -Iinclude -CAS_VERSION -g

stack_global_spinlock:
	gcc stack/spinlock_stack.c evaluate_stack.c -Iinclude -DSPINLOCK_VERSION -g

clean:
	rm build/