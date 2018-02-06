/*
 * @Author: bbdle 
 * @Date: 2018-02-06 19:34:03 
 * @Last Modified by: bbdle
 * @Last Modified time: 2018-02-06 19:48:58
 */

#ifndef __SOCKETLIBERRORS_H
#define __SOCKETLIBERRORS_H

#include "SocketLibTypes.h"
#include <exception>
#include <string>

namespace SocketLib
{
    enum Error
    {
        ESeriousError,

        ENetworkDown,
        ENoSocketsAvailable,
        ENoMemory,
        EAddressNotAvailable,
        EAlreadyConnected,
        ENotConnected,
        EConnectionRefused,
        ENetworkUnreachable,
        ENetworkReset,
        EHostUnreachable,
        EHostDown,
        EConnectionAborted,
        EConnectionReset,
        EOperationWouldBlock,

        EDNSNotFound,
        EDNSError,
        ENoDNSData,

        EInProgress,
        EInterrupted,
        EAccessDenied,
        EInvalidParameter,
        EAddressFamilyNotSupported,
        EProtocolFamilyNotSupported,
        EProtocolNotSupported,
        EProtocolNotSupportedBySocket,
        EOperationNotSupported,
        EInvalidSocketType,
        EInvalidSocket,
        EAddressRequired,
        EMessageTooLong,
        EBadProtocolOption,
        EAddressInUse,
        ETimedOut,
        EShutDown,

        ESocketLimitReached,
        ENotAvailable,
        EConnectionClosed
    };

    // ========================================================================
    //  translates error codes from the native platoform format into the SocketLib format
    // ========================================================================
    Error TranslateError(int p_error, bool p_errno);

    // ========================================================================
    //  just a wrapper
    // ========================================================================
    Error GetError(bool p_errno = true);

    // ========================================================================
    // a socket error class holds an error
    // ========================================================================
    class Exception : public std::exception
    {
    public:
        Exception(Error p_code);

        // return Error Code
        Error ErrorCode();

        // print readable message
        std::string PrintError();
    
    private:
        Error m_code;
    };
}


#endif