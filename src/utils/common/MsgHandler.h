/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2003-2019 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    MsgHandler.h
/// @author  Daniel Krajzewicz
/// @author  Michael Behrisch
/// @author  Jakob Erdmann
/// @date    Tue, 17 Jun 2003
/// @version $Id$
///
// Retrieves messages about the process and gives them further to output
/****************************************************************************/
#ifndef MsgHandler_h
#define MsgHandler_h


// ===========================================================================
// included modules
// ===========================================================================
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <utils/iodevices/OutputDevice.h>


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * MsgHandler
 */
class MsgHandler {
public:
    /**
     * @enum MsgType
     * An enumeration to differ between different types of messages
     * (errors, warning and information)
     */
    enum MsgType {
        /// The message is only something to show
        MT_MESSAGE,
        /// The message is a warning
        MT_WARNING,
        /// The message is an error
        MT_ERROR,
        /// The message is an debug
        MT_DEBUG,
        /// The message is an debug
        MT_GLDEBUG
    };

private:
    typedef MsgHandler* (*Factory)(MsgType);

public:
    /// @brief Sets the factory function to use for new MsgHandlers
    static void setFactory(Factory func) {
        // clean old instances
        cleanupOnEnd();
        myFactory = func;
    }

    /// @brief Returns the instance to add normal messages to
    static MsgHandler* getMessageInstance();

    /// @brief Returns the instance to add warnings to
    static MsgHandler* getWarningInstance();

    /// @brief Returns the instance to add errors to
    static MsgHandler* getErrorInstance();

    /// @brief Returns the instance to add debug to
    static MsgHandler* getDebugInstance();

    /// @brief Returns the instance to add GLdebug to
    static MsgHandler* getGLDebugInstance();

    /// @brief enable/disable debug messages
    static void enableDebugMessages(bool enable);

    /// @brief enable/disable gl-debug messages
    static void enableDebugGLMessages(bool enable);

    /// @brief check whether to enable/disable debug messages
    static inline bool writeDebugMessages() {
        return myWriteDebugMessages;
    }

    /// @brief check whether to enable/disable gl-debug messages
    static inline bool writeDebugGLMessages() {
        return myWriteDebugGLMessages;
    }

    /// @brief ensure that that given output device is no longer used as retriever by any instance
    static void removeRetrieverFromAllInstances(OutputDevice* out);

    ///@brief init output options
    static void initOutputOptions();

    /// @brief Removes pending handler
    static void cleanupOnEnd();

    /// @brief adds a new error to the list
    virtual void inform(std::string msg, bool addType = true);

    /// @brief adds a new formatted message
    // variadic function
    template<typename T, typename... Targs>
    void informf(const std::string& format, T value, Targs... Fargs) {
        if (myAggregationThreshold >= 0) {
            if (myAggregationCount[format]++ >= myAggregationThreshold) {
                return;
            }
        }
        (*this) << build("", true);
        _informf(format.c_str(), value, Fargs...);
    }

    /** @brief Begins a process information
     *
     * When a longer action is started, this method should be used to inform the user about it.
     * There will be no newline printed, but the message handler will be informed that
     *  a process message has been begun. If an error occurs, a newline will be printed.
     * After the action has been performed, use endProcessMsg to inform the user about it.
     */
    virtual void beginProcessMsg(std::string msg, bool addType = true);

    /// @brief Ends a process information
    virtual void endProcessMsg(std::string msg);

    /// @brief Clears information whether an error occurred previously
    virtual void clear();

    /// @brief Adds a further retriever to the instance responsible for a certain msg type
    virtual void addRetriever(OutputDevice* retriever);

    /// @brief Removes the retriever from the handler
    virtual void removeRetriever(OutputDevice* retriever);

    /// @brief Returns whether the given output device retrieves messages from the handler
    bool isRetriever(OutputDevice* retriever) const;

    /// @brief Returns the information whether any messages were added
    bool wasInformed() const;

    /** @brief Generic output operator
     * @return The MsgHandler for further processing
     */
    template <class T>
    MsgHandler& operator<<(const T& t) {
        // inform all other receivers
        for (OutputDevice* o : myRetrievers) {
            (*o) << t;
        }
        return *this;
    }

protected:
    /// @brief Builds the string which includes the mml-message type
    inline std::string build(const std::string& msg, bool addType) {
        if (addType) {
            switch (myType) {
                case MT_MESSAGE:
                    break;
                case MT_WARNING:
                    return "Warning: " + msg;
                    break;
                case MT_ERROR:
                    return "Error: " + msg;
                    break;
                case MT_DEBUG:
                    return "Debug: " + msg;
                    break;
                case MT_GLDEBUG:
                    return "GLDebug: " + msg;
                    break;
                default:
                    break;
            }
        }
        return msg;
    }

    void _informf(const char* format) {
        inform(format, false);
    }

    /// @brief adds a new formatted message
    // variadic function
    template<typename T, typename... Targs>
    void _informf(const char* format, T value, Targs... Fargs) {
        for (; *format != '\0'; format++) {
            if (*format == '%') {
                (*this) << value;
                _informf(format+1, Fargs...); // recursive call
                return;
            }
            (*this) << *format;
        }
    }

    void setAggregationThreshold(const int thresh) {
        myAggregationThreshold = thresh;
    }

    /// @brief standard constructor
    MsgHandler(MsgType type);

    /// @brief destructor
    virtual ~MsgHandler();

private:
    /// @brief The function to call for new MsgHandlers, nullptr means use default constructor
    static Factory myFactory;

    /// @brief The instance to handle debug
    static MsgHandler* myDebugInstance;

    /// @brief The instance to handle glDebug
    static MsgHandler* myGLDebugInstance;

    /// @brief The instance to handle errors
    static MsgHandler* myErrorInstance;

    /// @brief The instance to handle warnings
    static MsgHandler* myWarningInstance;

    /// @brief The instance to handle normal messages
    static MsgHandler* myMessageInstance;

    /// @brief Information whether a process information is printed to cout
    static bool myAmProcessingProcess;

private:
    /// @brief The type of the instance
    MsgType myType;

    /// @brief information whether an output occurred at all
    bool myWasInformed;

    /// @brief do not output more messages of the same type if the count exceeds this threshold
    int myAggregationThreshold;

    /// @brief count for messages of the same type
    std::map<const std::string, int> myAggregationCount;

    /// @brief The list of retrievers that shall be informed about new messages or errors
    std::vector<OutputDevice*> myRetrievers;

private:
    /// @brief invalid copy constructor
    MsgHandler(const MsgHandler& s) = delete;

    /// @brief invalid assignment operator
    MsgHandler& operator=(const MsgHandler& s) = delete;

    /** @brief Flag to enable or disable debug GL Functions
     *
     * This value is used to show more internal information throught warning messages about certain operations
     */
    static bool myWriteDebugMessages;
    static bool myWriteDebugGLMessages;
};


// ===========================================================================
// global definitions
// ===========================================================================
#define WRITE_WARNING(msg) MsgHandler::getWarningInstance()->inform(msg);
#define WRITE_WARNINGF(...) MsgHandler::getWarningInstance()->informf(__VA_ARGS__);
#define WRITE_MESSAGE(msg) MsgHandler::getMessageInstance()->inform(msg);
#define PROGRESS_BEGIN_MESSAGE(msg) MsgHandler::getMessageInstance()->beginProcessMsg((msg) + std::string(" ..."));
#define PROGRESS_DONE_MESSAGE() MsgHandler::getMessageInstance()->endProcessMsg("done.");
#define PROGRESS_BEGIN_TIME_MESSAGE(msg) SysUtils::getCurrentMillis(); MsgHandler::getMessageInstance()->beginProcessMsg((msg) + std::string(" ..."));
#define PROGRESS_TIME_MESSAGE(before) MsgHandler::getMessageInstance()->endProcessMsg("done (" + toString(SysUtils::getCurrentMillis() - before) + "ms).");
#define PROGRESS_FAILED_MESSAGE() MsgHandler::getMessageInstance()->endProcessMsg("failed.");
#define WRITE_ERROR(msg) MsgHandler::getErrorInstance()->inform(msg);
#define WRITE_DEBUG(msg) if(MsgHandler::writeDebugMessages()){MsgHandler::getDebugInstance()->inform(msg);};
#define WRITE_GLDEBUG(msg) if(MsgHandler::writeDebugGLMessages()){MsgHandler::getGLDebugInstance()->inform(msg);};

#endif

/****************************************************************************/
