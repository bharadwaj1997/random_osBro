////////////Broker Headers///////////////////////
#include <broker/broker.hh>
#include <broker/endpoint.hh>
#include <broker/message_queue.hh>
#include <poll.h>
#include <iostream>
#include <unistd.h>
#include <iostream>
////////////Osquery Headers///////////////////////
#include <osquery/database.h>
#include <osquery/tables.h>
#include <osquery/sdk.h>
#include "/home/chenone2316/osquery/osquery/events/linux/inotify.h"
//////////////////////////////////////////////////////

/////////////////////////////////////////////////////
namespace osquery {
namespace tables {
broker::endpoint pc1("VM");

class NewETCFilesEventSubscriber  : public EventSubscriber<INotifyEventPublisher> 
{

 public:
  // Implement the pure virtual init interface.
  Status init()
  {
    broker::init();
    pc1.listen(9999,"192.168.1.187"); //own IP
    auto conn_status = pc1.incoming_connection_status().need_pop();
    for(auto cs : conn_status)
   {
        if(cs.status == broker::incoming_connection_status::tag::established)
        { 
            break;
        }
   }
        ///////////////////////////////////////////////////
    auto sc = createSubscriptionContext();
    sc->path = "/etc";
    sc->recursive = true;
  // 'mask' is specific to inotify.
    sc->mask = IN_CREATE;
    subscribe(&NewETCFilesEventSubscriber::Callback, sc,nullptr);
     return Status(0, "OK");
   }
  Status Callback(const INotifyEventContextRef& ec, const void* user_data)
   {
   pc1.send("Testing",broker::message{broker::to_string(ec->path),broker::to_string(ec->time_string)});
   Row r;
   r["path"] = ec->path;
   r["time"] = ec->time_string;
   add(r, ec->time);
   return Status(0, "OK");
   }
  };
  REGISTER(NewETCFilesEventSubscriber, "event_subscriber", "new_etc_files");
  
 }
}
