###Guide###
For these file to work you should have cloned osquery from github and working build with `make` command. Then follow 
the steps given below:
* `1st` step is copying the `new_etc_file.table` to the `<osquery/tables/specs/>` directory. It will add new table with columns given in the .table file. At this step you can build osquery with `make && make install` and then check your table in osqueryi shell with .table command to list you table.
* Copy  `new_etc_files.cpp`  in `<osquery/tables/events/linux/>`  directory. This file contain the functionality implementation with eventing framework for our table. 
* Copy the `CMakeList.txt` in  `<osquery/tables/>` and then build osquery again. In `CMakeList.txt`, we have added link to broker library.
* Up to this point, our table with its implementation is ready. Now you need to run osqueryd for scheduling your query, but before that make configuration file for `osqueryd`. Goto `</var/osquery/>` and then copy the osquery.conf in that directory.
* Finally run `osqueyd` and try adding new files in `/etc` directory and check `</var/osquery/osquery.d/>` directory. There you will see log files in JSON formate. 


*We send broker messages in the callback() defined in `new_etc_files.cpp`. *


###Two Way Communication Guide###
I have used the external builder to build the project. I have used `netbeans` but you may use any, at your ease like `gcc`, to build the code.  But you need to link `-std=c++11 -lbroker` at master side and `clang++ -std=c++11 -I/usr/local/include/ -losquery -lthrift -lboost_system -lboost_thread-mt -lboost_filesystem -lglog -lrocksdb -lbroker` at client side.

####Steps to Follow####
1- First compile  `clientmain.cpp` and `mastermain.cpp` at two different PCs.
2- Run `osqueryd` at client side.
3- Run master program first (to set it to listen connections) and then client program: you will see the query results.
#### Notes: make sure port 9999 is open (firewall settings). ####

 ####Code Working Steps####
 * Broker Connection is established. 
 * Master sends a `SQL` query.
 * Client Receives, processes that query (osqueryd) and finally sends results to master node. (client prints query results locally too.)
 * Master simply prints the received data.
