# bloom-cpp98
C++ Extensions Library for POSIX systems (over c++98).
Main Page {#mainpage}
=========
  Bloom-cpp98 is a C++ extensions library for POSIX systems 
(over c++98). This library developing for projects in witch 
no possibility or no desire to use Boost/C++11. Or you want
to use Bloom++ features.

  Bloom-cpp98 implemented a few C++11 features:
  - Smart pointers. (shared_ptr, unique_ptr)
  - thread.
  - mutex, scoped_lock(lock_guard), unique_lock.
  - condition_variable.
  - Containers (list, set, hash_table, string).
  
  Also some additional Bloom++ features.
  - Multi-Thread safe containers (mt_list, mt_set, mt_hash_table).
  - Functional logger.
  - Network library (Sockets, TCP Server/Clien, UDP Communicator).
  - Dynamic library ( IN DEVELOP ).
    Is a implementation of dynamic types, dynamic classes and objects.
  - Shared library ( IN DEVELOP ).
    Shared containers with shared objects providing:
      - to destroy object after removing from the last container.
      - to remove object from all containers which are contains it
      at time of destroying.
      - to remove object from all containers and destroy it after
      removing from any container.
  - FCGI-Server ( IN DEVELOP ).
  - ODBC Session ( IN DEVELOP ).
    Working with SQL servers using dynamic and shared libraries.
    
Author
------
*Sergei <Wilder> Khairulin <sergei.khairulin@gmail.com>*

  
