//
//  MsgSock.h
//  fantasybit
//
//  Created by Jay Berg on 4/6/14.
//
//

#ifndef __fantasybit__MsgSock__
#define __fantasybit__MsgSock__

#include <cstddef>
#include "nn.hpp"
#include <google/protobuf/message.h>

namespace fantasybit
{

class Receiver
{
    nn::socket &sock;
    //size_t bsize;
    void *buf;
public:
    //Receiver(nn::socket &s, char *b, std::size_t size) : sock{s}, bsize{size} , buf{b} {}
    Receiver(nn::socket &s) : sock{s} , buf(nullptr){} //, bsize{256}, buf{new char[bsize]} {}

    template <class T>
    bool receive(T &t,int flags =0)
    {
        int size = sock.recv(&buf, NN_MSG, flags);
        if ( size == -1)
            return false;
        t.ParseFromArray(buf, size);
        nn_freemsg(buf);
        return true;
    }

	template <class T>
	static bool Receive(nn::socket &rsock,T &t, int flags = 0)
	{
		void *sbuf;
		int size = rsock.recv(&sbuf, NN_MSG, flags);
		if (size == -1)
			return false;
		t.ParseFromArray(sbuf, size);
		nn_freemsg(sbuf);
		return true;
	}
};

class Sender
{
    nn::socket &sock;
public:
    Sender(nn::socket &s) : sock{s} {}
    
    int send(const google::protobuf::Message &msg)
    {
        return Send(sock,msg);
    }
    static int Send(nn::socket &s, const google::protobuf::Message &msg, int flags = 0);
};

}

#endif /* defined(__fantasybit__MsgSock__) */
