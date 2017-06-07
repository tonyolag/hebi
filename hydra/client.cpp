#include "client.hpp"

namespace hydra
{
    client::client(sockets::socket& pSock, session *pSess)
        : connection(pSock, pSess, thread::make_thread_ptr("hydra::client::run", &client::run, this))
    {
        mSrvId = 0;
    }

    void client::run()	
    {
        mRunning = true;
        bool welcomed = false;

        logs::debug << LOC() "Starting client" << logs::done;
        
        while (mSock.connected())
        {
            while(!welcomed)
            {
                mSock.send("KNOCK % %", mSess->nodeId(), "Hebi");

                message msg = connection::read();
                while (!msg.is(message::command::MEET))
                {
                    mSess->handle(msg);
                    msg = connection::read();
                }
                meet meetMsg = static_cast<meet>(msg.derived());
                if (meetMsg.id == mSess->nodeId())
                {
                    mSrvId = meetMsg.id;
                }

                msg = connection::read();
                while (!msg.is(message::command::WELCOME))
                {
                    mSess->handle(msg);
                    msg = connection::read();
                }
                welcome welcomeMsg = static_cast<welcome>(msg.derived());
                if (welcomeMsg.ind == mSess->nodeId())
                {
                    welcomed = true;
                }
            }

            message msg = connection::read();
            if (mSess->seen(msg))
            {
                logs::debug << "Ignoring seen message: " << msg.serialize() << logs::done;
                continue;
            }
            else if (msg.is(message::command::DROP))
            {
                drop dropMsg = static_cast<drop>(msg.derived());
                if (dropMsg.id == mSess->nodeId())
                {
                    welcomed = false;
                }
                else
                {
                    mSess->handle(msg);
                }
            }
            else
            {
                mSess->handle(msg);
            }
        }
        mRunning = false;

        logs::debug << LOC() "Cleaning up client" << logs::done;
    }

    void client::send(const message pMsg)
    {
        std::string msg = pMsg.serialize();
        mSock.send(msg.c_str());
    }

    bool client::operator==(const client& pSrv)
    {
        return mSock == pSrv.mSock;
    }

    client::~client()
    {
    }
}
