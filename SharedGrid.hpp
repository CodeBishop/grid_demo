#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>

struct SharedGrid {
   static const int SIZE_X = 30, SIZE_Y = 10;
   SharedGrid() : hasNewData(false) {}
   //Mutex to protect access to the queue
   boost::interprocess::interprocess_mutex mutex;
   //Condition to wait when the queue is empty
   boost::interprocess::interprocess_condition myCondEmpty;
   //Condition to wait when the queue is full
   boost::interprocess::interprocess_condition myCondFull;
   char cell[SIZE_X];//[SIZE_Y]; //cell to fill
   bool hasNewData; //Is there any message
};