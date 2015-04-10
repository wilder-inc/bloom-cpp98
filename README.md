###<img src=bloom++-logo.png> **C++** Features for **C++11** and **C++98**.

## bloom-cpp98
C++ Features Library for POSIX systems (over c++98).

### Description:

  **Bloom-cpp98** is a small library which contain many useful and simply
implemented featrures for **C++**. This library can help you to develop
multi-threaded applications, network application and database 
applications. Also bloom-cpp98 can be used to develop projects in 
which no possibility or desire to use **Boost/C++11** but you need some 
features of their.

  **Bloom-cpp98** implemented a few **C++11** features:
  - Smart pointers (**shared_ptr**, **unique_ptr**).
  - **thread**.
  - **mutex**, **scoped_lock**(lock_guard), **unique_lock**.
  - **condition_variable**.
  - Containers (**list**, **set**, **hash_table**, **string**).

  Also some additional **Bloom++** features:
  - Multi-Thread safe containers (**mt_list**, **mt_set**, **mt_hash_table**).
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
    
### Author:
**Sergei Khairulin** \<sergei.khairulin@gmail.com\>

  
