# Concurrency


## Protecting the shared data

### What is it?
If the data in your code is shared through different threads and those threads are allowed to mutate (write) it, then there is a danger of breaking the one or more conditions which satisfies the `invariance` of that data.

A thread might be writing (changing) a portion of a shared data, another thread might try to access the same portion of data which might lead incorrect behavior for the caller. The other extreme that seperate threads might access to modify the same shared data at the same time. This brings up the issue called `race condition`.

Race conditions are not always considered as bad i.e. adding data to a container through different threads. This is not a bad thing. Yes, each of them modifies the shared data but do not break the invariants.

For instance, when an element is removed from a `std::vector<T>` by index, several internal instructions take place to complete the operation.
- Iterate to the element at that index.
- Calls the destructor for the removed element.
- Remove it from the dynamic memory
- Update the memory layout for the internal representation.
- Update the size invariants.

Whether it is single or multi-threaded environment, satisfying back the invariants requires multiple steps. However, in a multi-threaded environment, if one of the threads is removing an element from the container while another thread is iterating through the container for reading purposes, the invariant is broken at the time of the reading data for the second thread. This `race condition` is dangerous and not easy to identify.

To avoid this type of race conditions:
- Using mutexes
- Lock-free programming
- Transaction based programming

### Using mutexes
This is a synchronization mechanism to avoid dangerous race conditions by providing a `lock` before accessing the shared data and `unlocking` it when the required work is done. This means that when a thread is working on this data, no other thread can access the same locked data and has to wait until the working thread is done.

```cpp
#include <mutex>
#include <stack>

std::stack<int> s;
std::mutex my_mutex; // protects s
void pop_from_stack()
{
	std::lock_guard<std::mutex> guard(my_mutex);
	s.pop();
}// the lock is released at this point through std::lock_guard destructor.
```

### Deadlock
When `race condition` almost means that multiple threads are racing with each other to be the first to operate on shared data, `deadlock` corresponds to that each thread is waiting for the other to complete or release the mutex.

#### **What causes deadlock?**
1. Waiting to release locked mutexes:

When a thread locks a mutex and concurrently at least one other thread locks another mutex, this cause one thread to wait for another. This is called `deadlock`. In other words, having multiple mutexes locked at the same time.

```cpp
std::mutex m1, m2;
int main()
{
	std::thread t1([&]()
	{
		while (true)
		{
			std::lock_guard<std::mutex> g1(m1);		// m1.lock();
			std::cout << std::this_thread::get_id() << "\n";
			std::lock_guard<std::mutex> g2(m2);		// m2.lock();
			std::cout << std::this_thread::get_id() << "\n";
			// m1.unlock();
			// m2.unlock();
		}
	});

	std::thread t2([&]()
	{
		while (true)
		{
            // Notice the incompatible ordering with t1's lambda
			std::lock_guard<std::mutex> g2(m2);		// m2.lock();			 
			std::cout << std::this_thread::get_id() << "\n";
			std::lock_guard<std::mutex> g1(m1);		// m1.lock();
			std::cout << std::this_thread::get_id() << "\n";
			//m1.unlock();
			//m2.unlock();
		}
	});

	t1.join();
	t2.join();
    return 0;
}
```

2. Having multiple threads w/o lock and call `join()` for each other. (Threads waiting for each other)

#### **How to avoid deadlocks?**
1. Lock and unlock mutexes in the same order.
```cpp
std::mutex m1, m2;
int main()
{
	std::thread t1([&]()
	{
		while (true)
		{
			std::lock_guard<std::mutex> g1(m1);		// m1.lock();
			std::cout << std::this_thread::get_id() << "\n";
			std::lock_guard<std::mutex> g2(m2);		// m2.lock();
			std::cout << std::this_thread::get_id() << "\n";
			// m1.unlock();
			// m2.unlock();
		}
	});

	std::thread t2([&]()
	{
		while (true)
		{
            // Notice the same ordering with t1's lambda
			std::lock_guard<std::mutex> g1(m1);		// m1.lock();			 
			std::cout << std::this_thread::get_id() << "\n";
			std::lock_guard<std::mutex> g2(m2);		// m2.lock();
			std::cout << std::this_thread::get_id() << "\n";
			//m1.unlock();
			//m2.unlock();
		}
	});
    
	t1.join();
	t2.join();
    return 0;
}
```
2. Use `std::lock` (C++11)

```cpp
std::mutex m1, m2;
int main()
{
    std::lock(m1, m2);
    std::lock_guard<std::mutex> g1(m1, std::adopt_lock);
    std::lock_guard<std::mutex> g2(m2, std::adopt_lock);
    // Do stuff
}
```
3. Use `std::scoped_lock` (C++17)
```cpp
std::mutex m1, m2;
int main()
{
    std::scoped_lock(m1, m2);
    // Do stuff
}
```

4. Using Lock Hierarchy
Refer to section 3.2.5 in [1].


When designing a class or a container, if concurrency is a concern, act meticilously for the order of the locking of the mutexes. Each operation for the class should be consistent in that sense to avoid `deadlock`.

5. Using `std::unique_lock`

Check out the section below for details.

### `std::unique_lock`
`std::uniqe_lock` brings some flexibility by providing more user control on the locks. `std::lock_guard` is initialized by locking the given mutex and unlocks the mutex when the destructor is called so the lock status of the mutex is dependent on the lifetime of the `std::lock_guard` instance. 

`std::unique_lock` can provide this capability too but also provides a deferred_lock logic which means that the given mutex can be lock later when it needs to be done[2].

```cpp
std::mutex m1, m2;

void do_things()
{
	std::unique_lock<std::mutex> l1(m1, std::defer_lock);
	std::unique_lock<std::mutex> l2(m2, std::defer_lock);
	/*
		...
		Do some other stuff
	*/
	std::lock(l1, l2); // now we need

	// do other stuff
}
```
A timeout logic can also be achieved with `std::unique_lock`.

```cpp

```

Another feature of `std::unique_lock` is transferring the ownerhip of the mutexes between scopes. Since the `std::unique_lock` is a move-only type, returning from a function or applying `std::move()` on a named variable of it will trigger move operation which means the transfer of the ownership of the mutex hold by moved lock instance.
```cpp
std::unique_lock<std::mutex> get_lock()
{
	extern std::mutex m;
	std::unique_lock<std::mutex> l(m);
	// do stuff to prepare data
	return l;
}

void work_on_data()
{
	std::unique_lock<std::mutex> lock(get_lock());
	work();
}
```

The major flexibility provided by `std::unique_lock` is the feature that provides you to decide when to lock() and unlock() based on your needs. You can *lock()* and *unlock()* when you want to. `std::lock_guard` does not have this option and it's lifetime and so unlocking depends on the scope it is defined within. 
```cpp
void func()
{
	std::unique_lock<std::mutex> l(m);
	my_data theData = get_data(); // make sure no other thread is working this data while fetching
	l.unlock(); // You are done. No need to keep the lock further until you need it next time.

	process_data(theData); // you worked on it.
	
	l.lock(); // You are gonna write it so need a lock.
	write_data(theData);
}
```


> There is a bit of size and performance penalty while using `std::unique_lock` since it requires additional storage and logic within to handle these flexibilities.


### Protecting shared data during initialization
Check `Call a function exactly onece even if called concurrently using std::once_flag and std::call_once` section

### Reader-writer mutex
There are some instances that exclusive lock (lock by only one thread) is only required during `write` operation and concurrent access by multiple threads is required for `read` operations. This idiom requires the usage of `std::shared_mutex`.
```cpp

std::shared_mutex m;
std::unordered_map<std::string, std::string> some_cache;

// Multiple threads can concurrently read data using std::shared_lock
// No other thread is blocked
std::string read(std::string key)
{
	std::shared_lock<std::shared_mutex> sl(m);
	return some_cache.count(key) ? some_cache[key] : "";
}

// When a single thread wants to write, it gains exclusive lock using std::lock_guard or std::uniqe_lock
// which also blocks other threads that calls read()
void write(std::string key, std::string value)
{
	std::unique_lock<std::shared_mutex> l(m);
	some_cache[key] = value;
}
```

### Best Practices
- Never pass pointers and/or reference of protected data by any means to outside of the scope of the lock.(i.e. to user supplied functions, by return value of a function)
- Define the shared data and the mutex within a class to provide the encapsulation.
- Make sure each thread acquires the locks on mutexes in a fixed order. Any variation in the order of the mutex locks cause `deadlock`.
- Use `std::lock` or `std::scoped_lock` if multiple mutexes need to be locked.
- `mutable std::mutex` idiom is legal and highly practiced.

## Synchronization between threads
### `std::condition_variable`

Use this approach when
- there is one or multiple threads are waiting for the same event to occur.
- (optional) the waiting threads have to respond to an occurred event. 


```cpp
std::vector<int> data;
std::condition_variable cv;
std::mutex m;
bool processed = false;
void worker_thread()
{
	std::cout << "Worker thread is waiting for the signal...\n";
	std::unique_lock<std::mutex> lock(m);
	cv.wait(lock, []() {return !data.empty(); }); // Wait until the data is populated.

	std::cout << "Worker thread is processing data...\n";
	processed = true;
	std::cout << "Worker thread is done with processing the data...\n";
	
	// Unlock the mutex before signaling to avoid waiting thread to lock the same
	// mutex again. 
	lock.unlock();
	cv.notify_one();
}

void notifier_thread()
{
	data.push_back(5);
	{
		std::lock_guard<std::mutex> l(m);
		std::cout << "Notifier thread signals()... \n";
	}
	cv.notify_one();

	// Wait for the worker thread
	{
		std::unique_lock<std::mutex> l(m);
		cv.wait(l, []() {return processed; });
	}
	std::cout << "Back in notifier thread. \n";
}

int main()
{
	std::thread worker(worker_thread), notifier(notifier_thread);
	worker.join(); notifier.join();
    return 0;
}
```

### `std::future` & `std::shared_future`
#### **What is it?**
As stated in [10]:

> a `std::future` is an asynchronous return object ("an object that reads results from a shared state")

These mechanisms are useful when the waiting thread(s) have to `wait only once` for the condition to be true and will `never have to wait again`. A `std::future<T>` instance is the only instance that refers to an event while multiple `std::shared_future<T>` instances can refer to the same event. The template argument of the `std::future<T>` or `std::shared_future<T>` corresponds to the type of the associated data with the event. If there is no data, use `void` specialization.

Futures are used to communicate between threads but they do NOT provide a built-in synchronized access so addtional steps have to be taken to access the data from multiple thread async. There are two options for this.
1. Use `std::mutex` or other mechanisms for protected access
2. The threads may access their own **copies** of `std::future` instances via `std::async`.

#### **Syntax**
- Using member functions (pointer to member function)
```cpp
struct my_struct
{
	void func(int x, const std::string& y) {}
	std::string boo(const std::string& par) { return "return_val"; }
};

my_struct obj;
auto f1 = std::async(&my_struct::func, &obj, 13, "func"); // calls on obj instance
auto f2 = std::async(&my_struct::boo, obj, "doesn't matter");// calls boo on temp (copy of obj) object
auto f3 = std::async(&my_struct::func, my_struct(), 34, "func"); // calls func on temp (move constructed) object
```

- Callable objects
```cpp
struct callable
{
	double operator() (double val) { return val * val; }
};

callable obj;
auto f1 = std::async(callable(), 3.45); // Calls operator(double) on move constructed temp object
auto f2 = std::async(std::ref(obj), 3.1); // call operator(double) on obj
```
#### **How it works?**
1. An instance of `std::future` or `std::shared_future` is assigned (or registered) to an event on the working thread.
2. This intiates the waiting.
3. (Optional) The waiting thread can do some other tasks while waiting.
4. Worker thread works for the event and completes it.
5. Marks the feature as *ready* so from now on the future can not be *reset*.
6. Call `get()` on future instance and this will block the caller thread until the future is marked as ready


#### **Where it runs?**
In order to determine where `std::async` runs, `std::launch` should be be provided as the first argument. The default is implementation dependent.
```cpp
// Runs in the caller thread
std::future<int> future1 = std::async(std::launch::deferred, background_calculation);
// Runs in a new thread
std::future<int> future2 = std::async(std::launch::async, background_calculation);
// Implementation dependent
std::future<int> future3 = std::async(background_calculation);
std::future<int> future4 = std::async(std::launch::deferred | std::launch::async, background_calculation); // same as above
```

#### **How to use?**
- Getting values from background tasks
```cpp
#include <future>
#include <algorithm>
#include <numeric>
#include <chrono>

int background_calculation()
{
	std::cout << "Background thread: " << std::this_thread::get_id() << "\n";
	std::vector<int> v(1000);
	std::generate(v.begin(), v.end(), [n = 0]() mutable { return n++; });
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(10s);
	return std::accumulate(v.begin(), v.end(), 0);
}

int main()
{
	std::cout << "Caller thread: " << std::this_thread::get_id() << "\n";
	std::future<int> future = std::async(background_calculation);
	// do_some_other_stuff();
	std::cout << "The answer is " << future.get() << "\n";
	return 0;
}
```

### `std::packaged_task`
`std::package_task<>` wraps a Callable object(function, lambda or class with operator() overload) and associates it to a `std::future`. Unless otherwise stated or the task is not transferred to another thread, the task is run on the same thread with the caller.

#### **How it works?**
When a task is invoked:
1. calls the associated callable object.
2. sets the future's status *ready*.

#### **Where it runs?**
It can run sequentially or concurrently. In order to run a task concurrently, pass it to a thread other than the caller thread.

- Sequential Run
```cpp
int calculation(int feed = 0)
{
	std::cout << "Running calculation on thread:\t" << std::this_thread::get_id() << "\n";
	std::vector<int> v(1000);
	std::generate(v.begin(), v.end(), [n = feed]() mutable { return n++; });
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(10s);
	std::cout << "Calculation completed.\n";
	return std::accumulate(v.begin(), v.end(), 0);
}

void run_task()
{
	std::cout << "Task's caller thread:\t\t\t" << std::this_thread::get_id() << "\n";
	std::packaged_task<int(int)> task(calculation);	
	std::future<int> result = task.get_future();
	task(5);
	std::cout << "Task result:\t\t\t" << result.get() << "\n";
}

void run_task_with_bind()
{
	std::cout << "Task's caller thread:\t\t" << std::this_thread::get_id() << "\n";
	std::packaged_task<int()> task(std::bind(calculation, 6));
	std::future<int> result = task.get_future();
	task();
	std::cout << "Task result:\t\t\t" << result.get() << "\n";
}

/*
Task's caller thread:           24176
Running calculation on thread:  24176
Calculation completed.
Task result:                    504500
*/
```

- Concurrent run
```cpp
void run_task_on_other_thread()
{
	std::cout << "Task's caller thread:\t\t" << std::this_thread::get_id() << "\n";
	std::packaged_task<int()> task(std::bind(calculation, 6));
	std::future<int> result = task.get_future();
	
	// Transfer to another thread
	std::thread worker(std::move(task));
	// do_something_on_caller_thread();
	std::cout << "Task result:\t\t\t" << result.get() << "\n"; // wait for the result
	worker.join(); // wait for the thread completion	
}

/*
Task's caller thread:           23560
Running calculation on thread:  17472
Calculation completed.
Task result:                    505500	
*/
```

### `std::promise`
#### **What is it?**
As stated in [10]:
> `std::promise` is an asynchronous provider ("an object that provides a result to a shared state") i.e. a promise is the thing that you set a result on, so that you can get it from the associated future.

Unlike `std::packaged_task`, it does not wraps a callable object. You can consider `std::promise` as a communicator which is passed to a callable object to get the result (value or exception) of that executed callable object. It is kind of a mediator. The only way to get to know about the result of the operation is `std::future` member of the `std::promise`. The caller thread can communicate with the operation result through this future.

It is a move only type so can not be copied between threads or any other scope. Pass the named promises with `std::move()`.

#### **How it works?**
1. Create an `std::promise<T>` where *T* is the type of the result.
2. Get the `std::future` from the promise.
3. Now, this future is the way to get the result.
4. Pass the promise to the corresponding callable object so when the operation is complete, the required result will be set by passed promise.
5. Setting the result is actually updating the state of the future so from now on, the caller thread can request the result from the provided future (step 2) by *get()*.
6. When the callable objects does its thing, the caller thread might do its own thing.
7. The caller thread can ask the result through the given future by calling *get()*. If the result is ready returns immediately otherwise waits until the result is set. Calling *get()* on the future instance will block the caller thread until it gets a result.
8. (Optional) Caller thread joins the worker_thread.

#### **Where it runs?**
It can run sequentially or concurrently but it does not make any sense to run it sequentially. In order to run a task concurrently, pass it to a thread other as a part of the callable object that it will work for.

#### **How to use?**
- Success
```cpp
void calculation_with_promise(std::promise<int> calc_prom, int feed = 0)
{
	std::cout << "Running calculation on thread:\t" << std::this_thread::get_id() << "\n";
	std::vector<int> v(1000);
	std::generate(v.begin(), v.end(), [n = feed]() mutable { return n++; });
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(5s);
	std::cout << "Calculation completed.\n";
	calc_prom.set_value(std::accumulate(v.begin(), v.end(), 0));
}

int main()
{
	std::cout << "Task's caller thread:\t\t" << std::this_thread::get_id() << "\n";
	std::promise<int> calc_promise;
	std::future<int> calc_future = calc_promise.get_future();

	std::thread worker(calculation_with_promise, std::move(calc_promise), 4 );
	//do_something_on_caller_thread();	
	std::cout << "Task result:\t\t\t" << calc_future.get() << "\n";
	worker.join();	
	return 0;
}
/*
Task's caller thread:           28500
Running calculation on thread:  24388
Calculation completed.
Task result:                    503500
*/
```

- Handling exceptions
If an exception is thrown during the callable operation, it can be propagated through `set_exception` method on given promise.
```cpp

void calculation_with_promise_throws(std::promise<int> throws_promise)
{
	try
	{
		throw std::runtime_error("Example");
	}
	catch (...)
	{
		try
		{
			throws_promise.set_exception(std::current_exception());
		}
		catch (...) { }
	}
}
// a bit cleaner version
void calculation_with_promise_throws(std::promise<int> throws_promise)
{
	try
	{
		throw std::runtime_error("Example");
	}
	catch (...)
	{
		throws_promise.set_exception(std::make_exception_ptr(std::logic_error("foo")));
	}
}

int main()
{
	std::cout << "Task's caller thread:\t\t" << std::this_thread::get_id() << "\n";
	std::promise<int> calc_promise;
	std::future<int> calc_future = calc_promise.get_future();

	std::thread worker(calculation_with_promise_throws, std::move(calc_promise));
	//do_something_on_caller_thread();	
	try
	{
		std::cout << "Task result:\t\t\t" << calc_future.get() << "\n";
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception from the thread: " << e.what() << '\n';
	}
	
	worker.join();	
	return 0;
}
```

#### **Dealing with multiple threads**
- Multiple threads waiting for the same shared state using `std::shared_future`.

```cpp
int main()
{
	std::promise<void> global_promise, thread1_promise, thread2_promise;
	std::shared_future<void> shared_future(global_promise.get_future());
	std::chrono::time_point<std::chrono::high_resolution_clock> start;

	// Note that shared_future is passed by value which means copy
	auto do_stuff_on_thread1 = [&, shared_future]()->std::chrono::duration<double, std::milli>
	{
		thread1_promise.set_value();
		shared_future.wait(); // wait for the caller thread to signal
		return std::chrono::high_resolution_clock::now() - start;
	};

	auto do_stuff_on_thread2 = [&, shared_future]()->std::chrono::duration<double, std::milli>
	{
		thread2_promise.set_value();
		shared_future.wait(); // wait for the caller thread to signal
		return std::chrono::high_resolution_clock::now() - start;
	};

	// Get the futures for the threads
	auto fut1 = thread1_promise.get_future();
	auto fut2 = thread2_promise.get_future();
	
	// Run the threads
	auto result1 = std::async(std::launch::async, do_stuff_on_thread1);
	auto result2 = std::async(std::launch::async, do_stuff_on_thread2);

	// wait for the threads to become ready
	fut1.wait(); fut2.wait();

	// Now the threads are ready and waiting for the signal
	//, start the clock
	start = std::chrono::high_resolution_clock::now();
	// signal the threads
	global_promise.set_value();

	// Get the results
	std::cout << "Thread 1 received the signal "
		<< result1.get().count() << " ms after start\n"
		<< "Thread 2 received the signal "
		<< result2.get().count() << " ms after start\n";

	return 0;
}

/*
Thread 1 received the signal 0.0183 ms after start
Thread 2 received the signal 0.0161 ms after start
*/
```


## Misc
### `thread_local` storage specifier
Identifies the specified variable as `local to the running thread` but still global within that thread. It means that each thread has its own local copy.

```cpp
thread_local int g_var = 1;
// 
class MyClass{
public:
	thread_local static unsigned long mutex_id;
};

thread_local unsigned long mutex_id = LONG_MAX;

```
### Local static variables
Local scoped static variables are `thread-safe` by default. They can only be initialized by one thread and all the waiting threads should wait until the initialization is done.

### Call a function exactly once even if it is accessed concurrently using `std::once_flag` and `std::call_once`
A method can be called only and only one time even if it is called from different threads by marking a method with `std::once_flag` flag within `std::call_once`. 
```cpp
#include <mutex>
std::once_flag flag1;
void to_be_called_once()
{
	std::call_once(flag1, [](){std::cout << "This is the first and last call" << "\n";});
}
int main()
{
	std::thread t1(to_be_called_once);
	std::thread t2(to_be_called_once);
	std::thread t3(to_be_called_once);
	std::thread t4(to_be_called_once);
	t1.join(); t2.join(); t3.join(); t4.join();
}
// Output:
// "This is the first and last call"
```

If the called function throws, given `std::once_flag` instance is never flipped which means that the call is considered as `never made` so one of the subsequent attempts might result with call and that time the the flag is flipped. ([Example](https://en.cppreference.com/w/cpp/thread/call_once))

### Thread-safe Lazy initialization
`std::once_flag` and `std::call_once` together.
```cpp
class MyClass
{
private:
	std::once_flag init_flag_;
	std::shared_ptr<HugeData> resource_;
	void initialize()
	{
		resource_ = std::make_shared<HugeData>();
	}
public:
	void process()
	{
		std::call_once(init_flag_, &MyClass::initialize, this);
		resource_->process();
	}

	void notify()
	{
		std::call_once(init_flag_, &MyClass::initialize, this);
	}
};
``` 

## References
[1] C++ Concurrency In Action, 2nd edition, Anthony Williams

[[2] http://jakascorner.com/blog/2016/02/lock_guard-and-unique_lock.html](http://jakascorner.com/blog/2016/02/lock_guard-and-unique_lock.html)

[[3] cppreference.com](cppreference.com)

[[4] https://www.nextptr.com/question/qa1456812373/move-stdunique_lock-to-transfer-lock-ownership](https://www.nextptr.com/question/qa1456812373/move-stdunique_lock-to-transfer-lock-ownership)

[[5] http://www.gerald-fahrnholz.eu/sw/online_doc_multithreading/html/group___grp_mutexes_and_locks.html](http://www.gerald-fahrnholz.eu/sw/online_doc_multithreading/html/group___grp_mutexes_and_locks.html)

[[6] http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2406.html](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2406.html)

[[7] https://www.justsoftwaresolutions.co.uk/threading/multithreading-in-c++0x-part-5-flexible-locking.html](https://www.justsoftwaresolutions.co.uk/threading/multithreading-in-c++0x-part-5-flexible-locking.html)

[[8] https://mfreiholz.de/posts/make-a-class-thread-safe-cpp/](https://mfreiholz.de/posts/make-a-class-thread-safe-cpp/)

[[9] https://www.youtube.com/watch?v=WDjfcbxIlVQ&feature=youtu.be](https://www.youtube.com/watch?v=WDjfcbxIlVQ&feature=youtu.be)

[[10] What is std::promise?](https://stackoverflow.com/questions/11004273/what-is-stdpromise) 

