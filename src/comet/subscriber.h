/*
Copyright (c) 2012-2014 The icomet Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef ICOMET_SUBSCRIBER_H
#define ICOMET_SUBSCRIBER_H

#include "../build.h"
#include <string>
#include <evhttp.h>

class Server;
class Channel;


class Subscriber{
public:
	enum Type{
		POLL	= 0,
		STREAM	= 1,
		IFRAME	= 2
	};
public:
	Subscriber();
	~Subscriber();
	
	Subscriber *prev;
	Subscriber *next;
	
	Channel *channel;
	std::string callback;
	int type;
	int idle;
	// 当类型是 data 时的 seq, 和类型是 noop 时用的不是同一套 seq.
	int seq_next;
	// seq_noop 用于 js long-polling 判断重复连接
	int seq_noop;
	struct evhttp_request *req;

	void start();
	void close();
	void send_chunk(int seq, const char *type, const char *content);
	
	void noop();
	void send_old_msgs();
	void sync_next_seq();
	
	static void send_error_reply(int sub_type, struct evhttp_request *req, const char *cb, const std::string &cname, const char *type, const char *content);
	
private:
	void poll_send_start(bool array=true);
	void poll_send_end(bool array=true);
	void poll_send(int seq, const char *type, const char *content, bool array=true);
};

#endif
