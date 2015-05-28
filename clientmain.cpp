/* 
 * File:   main.cpp
 * Author: root
 *Clinet side code....... osquery side
 * Created on April 27, 2015, 5:11 PM
 */

#include <cstdlib>
#include <broker/broker.hh>
#include <broker/endpoint.hh>
#include <broker/message_queue.hh>
#include <poll.h>
#include <iostream>
/*
 * 
 */
int main() 
{
    broker::init();
    broker::endpoint pc1("VM");
    pc1.listen(9999,"192.168.1.90");
    broker::endpoint pc2("Main_PC");
    pc2.peer("192.168.1.187",9999);
    broker::message_queue mq("Testing",pc2);
    pollfd pfd{mq.fd() ,POLLIN, 0};
    
    int rv;
    rv=poll(&pfd, 1, 10000);
    if (rv == -1)
    {
        std::cout<< "Error..\n";
    }
    else if (rv == 0)
    {
    std::cout<<"Timeout occurred!  No data after 3.5 seconds.\n";
    } 
    else
    {
     for (auto& msg : mq.want_pop() )
     {
         std::cout << broker::to_string(msg);
     }
    }
    return 0;
    
}
