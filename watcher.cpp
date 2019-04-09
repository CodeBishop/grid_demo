// Compile with: g++ -o watcher watcher.cpp -lrt -pthread
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <iostream>
#include <cstring>
#include "SharedGrid.hpp"

using namespace boost::interprocess;

int main() {
   // Open the a shared memory object.
   shared_memory_object shm(open_only, "MySharedMemory", read_write);

   try {
      // Map the whole shared memory in this process
      mapped_region myMappedRegion(shm, read_write);

      // Get the address of the mapped region
      void *addr = myMappedRegion.get_address();

      // Obtain a pointer to the shared structure
      SharedGrid *data = static_cast<SharedGrid*>(addr);

      // Print messages until the other process marks the end
      bool exitFlag = false;
      while(!exitFlag) {
         scoped_lock<interprocess_mutex> lock(data->mutex);
         if(!data->hasNewData) {
            data->myCondEmpty.wait(lock);
         }
         if(std::strcmp(data->cell, "final message marker") == 0) {
            exitFlag = true;
         } else {
            // Print the message
            std::cout << data->cell << std::endl;
            // Notify the other process that the buffer is empty
            data->hasNewData = false;
            data->myCondFull.notify_one();
         }
      }
   }
   catch(interprocess_exception &ex){
      std::cout << ex.what() << std::endl;
      return 1;
   }

   return 0;
}