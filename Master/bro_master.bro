
module osquery;

const broker_port: port = 9999/tcp &redef;
redef exit_only_after_terminate = T;
redef BrokerComm::endpoint_name = "Printer";


type usb_devices: record {
	host_name:	string	&log;
	query_name:	string	&log;
	usb_address:	string	&log;
	usb_port:	string	&log;
	vendor:		string	&log;
	vendor_id:	string	&log;
	model:		string	&log;
	model_id:	string	&log;
	serial:		string	&log;
	removable:	string	&log;
	update:		string	&log;
};

export {
	redef enum Log::ID += {LOG};
	global usb_device_table: set[usb_devices];
}


event bro_init()
{
	BrokerComm::enable();
	BrokerComm::subscribe_to_prints("Testing");
	BrokerComm::listen(broker_port, ""); 
	Log::create_stream(LOG, [$columns=usb_devices,$path="osquery"]);
}

event BrokerComm::incoming_connection_established(peer_name: string)
{
	print "BrokerComm::incoming_connection_establisted", peer_name;
	BrokerComm::print("Testing","SELECT * FROM usb_devices");
}

event BrokerComm::print_handler(msg: string)
{
	local temp = msg;
	local test_pattern = /!/;
	if(test_pattern in temp)
	{
		local results = split(temp, test_pattern);
		if(results[11]=="Added")
		{
		add usb_device_table[[$host_name=results[1],$query_name=results[2],$usb_address=results[3],$usb_port=results[4],$vendor=results[5],$vendor_id=results[6],$model=results[7],$model_id=results[8],$serial=results[9],$removable=results[10],$update=results[11]]];
		}
		else
		{
		delete usb_device_table[[$host_name=results[1],$query_name=results[2],$usb_address=results[3],$usb_port=results[4],$vendor=results[5],$vendor_id=results[6],$model=results[7],$model_id=results[8],$serial=results[9],$removable=results[10],$update=results[11]]];
		}
		Log::write (osquery::LOG,[$host_name=results[1],$query_name=results[2],$usb_address=results[3],$usb_port=results[4],$vendor=results[5],$vendor_id=results[6],$model=results[7],$model_id=results[8],$serial=results[9],$removable=results[10], $update=results[11]]);
	}
}

event BrokerComm::incoming_connection_broken(peer_name: string)
{
	print "BrokerComm::incoming_connection_broken", peer_name;
	print usb_device_table;
	terminate();
}

