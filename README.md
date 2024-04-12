# C_THREAD_POOL
> a sample implement of thread pool in C language.

## Config

### cmake
If your cmake version is less than 3.29, 
you can try change the version to fit yours.

### Thread
If your system don't support the posix thread, 
comment the `THREADPOOL_POSIX` define marco.

### Pool
It's better to change the pool `MAX_THREAD` and `MAX_TASK`
to fit your cpu and have the best performance.


## Run 
```bash
git clone ~The URL~
cd ~The Dir~
mkdir build
cd build
cmake ..
cmake --build . --config Release
./ThreadPool
```