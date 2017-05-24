#include "node.hpp"

namespace hydra
{
    node::node(sockets::socket pSock)
        : mSock(pSock)
    {
    }

    message node::read()
    {
        while (mQueue.empty())
        {
            std::vector<std::string> lines = mSock.recv();
            for (std::string line : lines)
            {
                try
                {
                    message msg = message(line);
                    mQueue.push(msg);
                }
                catch (const std::invalid_argument&)
                {
                }
            }
        }
        return mQueue.pop();
    }
    
    message node::expect(message::command pCmd)
    {
        while (mSock.connected())
        {
            std::vector<std::string> lines = mSock.recv();
            for (std::string line : lines)
            {
                try
                {
                    message msg = message(line);
                    mQueue.push(msg);
                }
                catch (const std::invalid_argument&)
                {
                }
            }
            while (mQueue.notEmpty())
            {
                message msg = mQueue.pop();
                if (msg.is(pCmd))
                {
                    return msg;
                }
            }
        }
        return message("");
    }

    void node::run()
    {
        for(;;)
        {
        }
    }
}