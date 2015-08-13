
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
#include <pthread.h>
/*
 * 
 */
//Global Varibales
int thread_count=0;
broker::endpoint pc2("Printer");
broker::message_queue mq("Testing",pc2);
pollfd pfd{mq.fd() ,POLLIN, 0};

using namespace std;

void *remoteQuery(void *in_query)
{
    
    std::cout<<"Starting new Remote Query..."<<std::endl;
   // mq[thread_count] = broker::message_queue("Testing",pc2);
  //  pollfd pfd{mq[thread_count].fd() ,POLLIN, 0};
    
    std::string* query = reinterpret_cast<std::string*>(in_query);
    
    usleep(500000);
    pc2.send("Testing",broker::message{broker::to_string(*query)});
    usleep(500000);
    
    std::cout<<"Query Sent..."<<std::endl;
    
    pthread_exit(NULL);
}

void *run(void *data)
{
    pthread_t query_thread[5];
    string inString;
    string* inputQuery;
    int rc[5];
    
    while(thread_count<5)
    {
        std::cout<<"Please Enter SQL query String"<<std::endl;
        std::getline(cin,inString);
      //  cout<<inString<<endl;
        inputQuery = new string(inString); 
        rc[thread_count] = pthread_create(&query_thread[thread_count],NULL,remoteQuery,(void*)inputQuery);
        if(rc[thread_count])
        {
            std::cout<<"Error in pthread_create"<<std::endl;
            exit(-1);
        }
        thread_count++;
    }
    pthread_exit(NULL);
}

int main() 
{
    pthread_t run_thread;
    
    long t=0;
    
    broker::init();
    pc2.listen(9999,"");
   
    
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
    int rc = pthread_create(&run_thread,NULL,run,(void*)t);
    if(rc)
    {
        std::cout<<"Error in pthread_create"<<std::endl;
        exit(-1);
    }
    
    int rv;
    while(true)
    {
        rv=poll(&pfd, 1, 10000);
        if ((rv != -1) && (rv != 0))
        {
         for (auto& msg : mq.need_pop() )
         {
           std::string broker_msg = broker::to_string(msg);
           if(broker_msg.size()>2)
           {
            broker_msg = broker_msg.substr(1,broker_msg.size()-2);
            std::cout << broker::to_string(broker_msg)<<std::endl;
           }
         }
        }
    } 
    /*
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
    }*/
    return 0;
    
}
