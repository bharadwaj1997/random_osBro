**Guide**
For these file to work you should have cloned osquery from github and working build with *_make_* command. Then follow 
the steps given below:
* Item 1st step is copy the _new_etc_file.table_ to the <ruby osquery/tables/specs/> directory. It will add new table with columns given in the .table file. At this step you can build osquery with *_make && make install_* and then check your table in osqueryi shell with .table command to list you table.
* Copy  _new_etc_files.cpp_  in <osquery/tables/events/linux/>  directory. This file contain the functionality implementation with eventing framework for our table. 
* Copy the CMakeList.txt in  <osquery/tables/> and then build osquery again. In *CMakeList.txt*, we have added link to broker library.
* Up to this point, our table with its implementation is ready. Now you need to run osqueryd for scheduling your query, but before that make configuration file for osqueryd. Goto </var/osquery/> and then copy the osquery.conf in that directory.
* Finally run *_osqueyd_* and try adding new files in /etc directory and check </var/osquery/osquery.d/> directory. There you will see log files in JSON formate. 


*We send broker messages in the callback() defined in _new_etc_files.cpp_. *
