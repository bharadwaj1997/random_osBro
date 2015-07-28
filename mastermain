
#include <broker/message.hh>


#include <broker/address.hh>

#include <cstdlib>
#include <broker/broker.hh>
#include <broker/endpoint.hh>
#include <broker/message_queue.hh>
#include <poll.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
/*
 * 
 */
using namespace std;
int main() 
{
    static int count = 0;
    broker::init();
    broker::endpoint pc2("Printer");
    pc2.listen(9999,"");
   // broker::endpoint pc1("Testing");
   // pc1.peer("192.168.1.187",8080);
    
    std::cout<<"Listing"<<std::endl;
    auto conn_status = pc2.incoming_connection_status().need_pop();
    std::cout<<"Out of Pop function"<<std::endl;
    for (auto cs : conn_status)
    {
        if(cs.status == broker::incoming_connection_status::tag::established)
        {
            std::cout<< "Connection Established"<<std::endl;  
        }
        else
        {
            std::cout<<"Connection could not Establish"<<std::endl;
        }
    }
    
    broker::message_queue mq("Testing",pc2);
    pollfd pfd{mq.fd() ,POLLIN, 0};
    while(true)
    {
        usleep(500000);
        pc2.send("Testing",broker::message{"SELECT * FROM time"});
        usleep(500000);
        int rv;
        while(count!=1)
        {
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
             for (auto& msg : mq.need_pop() )
             {
               std::string broker_msg = broker::to_string(msg);
               if(broker_msg.size()>2)
               {
                broker_msg = broker_msg.substr(1,broker_msg.size()-2);
                std::cout << broker::to_string(broker_msg)<<std::endl;
                count = 1;
               }
             }
            }
        }
        count=0;
    }
    return 0;
    
}
