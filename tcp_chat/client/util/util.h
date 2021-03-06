#ifndef _UTIL_H_
#define _UTIL_H_

#include <iostream>
#include <string>
#include <stdlib.h>
#include <map>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "my_json.h"
using namespace std;

void log_error(const char*fmt, ...);
void log_message(const char*fmt, ...);

#define BUF_SIZE 512

//for server
typedef struct msg_info {
		
	msg_info()
	{
		_head._id = -1;
		_head._msglen = -1;
	}

	//头部在传输的时候占8个字节（_id占4个字节, _msglen占四个字节）
	enum {HEADLEN = 8};
	typedef struct {
		int		_id;	//channel id
		int		_msglen;
	} _head_info;

	_head_info	_head;
	string		_msg;

} msg_info;

//for channel

#define CMD_ONLINE 		1
#define CMD_REGISTER	2
#define CMD_MSG			3
#define CMD_CLIENT_INFO	4
#define CMD_USER_LIST	5

typedef struct data_info {

	data_info()
		: _id(-1), _cmd(-1), _datalen(-1)
	{}
	static void val_to_str(const data_info &data, string &str){
		Json::Value val;
		char buf[10];
		bzero(buf, sizeof(buf));
		sprintf(buf, "%d", data._id);
		val["_id"] = buf;
		sprintf(buf, "%d", data._datalen);
		val["_datalen"] = buf;
		val["_data"] = data._data.c_str();
		sprintf(buf, "%d", data._cmd);
		val["_cmd"] = data._cmd;
		my_json::val_to_str(val, str);
	}
	
	static int str_to_val(const string &str, data_info &data){
		Json::Value val;
		int ret = my_json::str_to_val(str, val);
		if(ret < 0){
			return ret;
		}
		data._id = atoi(val["_id"].asString().c_str());
		data._datalen = atoi(val["_datalen"].asString().c_str());
		data._data = val["_data"].asString();
		data._cmd = atoi(val["_cmd"].asString().c_str());
		return 0;
	}

	typedef	int		id_type;

	id_type		_id;	//user id
	int			_cmd;
	int			_datalen;
	string		_data;

} data_info;

typedef struct client_info {

	client_info()
		: _id(-1), _channel_id(-1) 
	{}

	static void val_to_str(const client_info &cli, string &str){
		Json::Value val;
		char buf[10];
		bzero(buf, sizeof(buf));
		sprintf(buf, "%d", cli._id);
		val["_id"] = buf;
		sprintf(buf, "%d", cli._channel_id);
		val["_channel_id"] = buf;
		val["_name"] = cli._name.c_str();
		my_json::val_to_str(val, str);
	}
	
	static int str_to_val(const string &str, client_info &cli){
		Json::Value val;
		int ret = my_json::str_to_val(str, val);
		if(ret < 0){
			return ret;
		}
		cli._id = atoi(val["_id"].asString().c_str());
		cli._channel_id = atoi(val["_channel_id"].asString().c_str());
		cli._name = val["_name"].asString();

		return 0;
	}

	typedef int		id_type;

	id_type				_id;			//user id
	id_type				_channel_id;	//组内id
	string 				_name;			//名字

} client_info;


#endif
