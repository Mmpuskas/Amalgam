#ifndef PEER_H_
#define PEER_H_

#include "NetworkDefs.h"
#include <SDL2/SDL_net.h>
#include <memory>
#include <array>

namespace AM
{

/**
 * Represents a network peer for communication.
 */
class Peer
{
public:
    /** Largest message we'll accept. Kept at 1450 for now to try to avoid IP fragmentation.
     *  Can rethink if we need larger. */
    static constexpr unsigned int MAX_MESSAGE_SIZE = 1450;

    /**
     * Initiates a TCP connection that the other side can then accept.
     * (e.g. the client connecting to the server)
     */
    static std::unique_ptr<Peer> initiate(std::string serverIP, unsigned int serverPort);

    /**
     * Constructor for when you only need 1 peer (client connecting to server, anyone
     * connecting to chat server.)
     * Constructs a socket set for this peer to use.
     */
    Peer(TCPsocket inSocket);

    /**
     * Constructor for when you need a set of peers (server connecting to clients).
     * Adds the socket to the given set.
     */
    Peer(TCPsocket inSocket, const std::shared_ptr<SDLNet_SocketSet>& inSet);

    ~Peer();

    /**
     * Returns false if the client was at some point found to be disconnected, else true.
     */
    bool isConnected() const;

    /**
     * Sends a message to this Peer.
     * Will error if the message size is larger than a Uint16 can hold.
     * @return Disconnected if the peer was found to be disconnected, else Success.
     */
    NetworkResult send(const BinaryBufferSharedPtr& message);

    /**
     * Tries to receive bytes over the network.
     *
     * @param checkSockets  If true, will call CheckSockets() before checking
     *                      SocketReady(). Set this to false if you're going to call
     *                      CheckSockets() yourself.
     * @param numBytes  The number of bytes to receive.
     * @return An appropriate ReceiveResult if the receive failed, else a ReceiveResult with
     *         result == Success and data in the message field.
     */
    ReceiveResult receiveBytes(Uint16 numBytes, bool checkSockets);

    /**
     * Returns the requested number of bytes, waiting if they're not yet available.
     * @param numBytes  The number of bytes to receive.
     * @return A message.
     */
    ReceiveResult receiveBytesWait(Uint16 numBytes);

    /**
     * Tries to receive a {size, message} pair over the network.
     *
     * @param checkSockets  If true, will call CheckSockets() before checking
     *                      SocketReady(). Set this to false if you're going to call
     *                      CheckSockets() yourself.
     * @return An appropriate ReceiveResult if the receive failed, else a ReceiveResult with
     *         result == Success and data in the message field.
     */
    ReceiveResult receiveMessage(bool checkSockets);

    /**
     * Receives a {size, message} pair and returns a message, waiting if the data is
     * not yet available.
     * @return A message.
     */
    ReceiveResult receiveMessageWait();

private:
    std::shared_ptr<SDLNet_SocketSet> set;
    TCPsocket socket;

    /**
     * Tracks whether or not this peer is connected. Is set to false if a disconnect
     * was detected when trying to send or receive.
     */
    bool bIsConnected;

    std::array<Uint8, MAX_MESSAGE_SIZE> messageBuffer;
};

} /* End namespace AM */

#endif /* End PEER_H_ */
