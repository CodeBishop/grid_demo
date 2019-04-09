// Compile with: g++ -o main main.cpp -lrt -pthread
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <iostream>
#include <cstdio>
#include "SharedGrid.hpp"

using namespace boost::interprocess;

int main() {
   // Erase previous shared memory in case it accidentally still exists
   shared_memory_object::remove("MySharedMemory");

   // Create a shared memory object.
   shared_memory_object shm(create_only, "MySharedMemory", read_write);
   std::cout << "Created shm. Size is " << sizeof(shm) << "bytes\n";
   try {
      std::cout << "Truncating shared mem to size: " << sizeof(SharedGrid) << "bytes\n";
      shm.truncate(sizeof(SharedGrid)); // Set size

      // Map the whole shared memory in this process
      mapped_region region(shm, read_write);

      // Get the address of the mapped region
      void *addr = region.get_address();

      // Construct the shared structure in memory
      SharedGrid * sharedArrayInstance = new (addr) SharedGrid;

      const int NumMsg = 7;

      for(int i = 0; i <= NumMsg; i++){
         scoped_lock<interprocess_mutex> lock(sharedArrayInstance->mutex);
         if(sharedArrayInstance->hasNewData) {
            sharedArrayInstance->myCondFull.wait(lock);
         }
         if(i == (NumMsg)) {
            std::printf("final message marker\n");
            std::sprintf(sharedArrayInstance->cell, "%s", "final message marker");
         } else {
            std::printf("my_trace%d\n", i);
            std::sprintf(sharedArrayInstance->cell, "%s_%d", "my_trace", i);
         }

         // Notify the other process that there is a message
         sharedArrayInstance->myCondEmpty.notify_one();
         sharedArrayInstance->hasNewData = true;
      }
      shared_memory_object::remove("MySharedMemory"); // De-allocate
   }
   catch(interprocess_exception &ex) {
      std::cout << ex.what() << std::endl;
      return 1;
   }

   return 0;
}