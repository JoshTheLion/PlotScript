/* Adapted from
www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html
*/

#ifndef _MESSAGE_QUEUE_HPP_
#define _MESSAGE_QUEUE_HPP_


#include <queue>
#include <mutex>
#include <condition_variable>

template<typename MessageType>
class MessageQueue
{
public:

  // push message into queue, blocks until available
  void push(MessageType const& message)
  {
    std::unique_lock<std::mutex> lock(the_mutex);
    the_queue.push(message);
    lock.unlock();
    the_condition_variable.notify_one();
  }

  // check if queue is empty, blocks until available
  bool empty() const
  {
    std::lock_guard<std::mutex> lock(the_mutex);
    return the_queue.empty();
  }

  // pop message from queue, return false if queue is empty
  bool try_pop(MessageType& popped_value)
  {
    std::lock_guard<std::mutex> lock(the_mutex);
    if(the_queue.empty()){
			return false;
		}
        
    popped_value=the_queue.front();
    the_queue.pop();
    return true;
  }

  // pop message from queue, blocks until the queue is nonempty
  void wait_and_pop(MessageType& popped_value)
  {
    std::unique_lock<std::mutex> lock(the_mutex);
    while(the_queue.empty()){
			the_condition_variable.wait(lock);
		}

    popped_value=the_queue.front();
    the_queue.pop();
  }

private:

  std::queue<MessageType> the_queue;
  mutable std::mutex the_mutex;
  std::condition_variable the_condition_variable;

};

#endif // _MESSAGE_QUEUE_HPP_
