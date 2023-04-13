# a-simple-ThreadPool

## unique_lock vs lock_guard

>lock_guard可以获取一个mutex类型的所有权，当lock_guard对象创建时会直接上锁，在离开作用域时会自动解锁。

>unique_lock可以获取一个mutex类型的所有权，当unique_lock对象创建时可以指定是否上锁（在其构造中第二个参数指定为std::defer_lock即可延迟上锁），可以使用unique_lock对象进行手动上锁、解锁，在离开作用域时若为上锁状态则会自动解锁。


## join() vs detach

>join()可使当前线程等待所启动的线程执行完成才会继续往下执行

>detach()可使当前线程无需等待所启动的线程执行完成，可以直接往下执行



