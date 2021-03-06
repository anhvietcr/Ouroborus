/*
 * Copyright 2020 Phara0h
 * Changes are the diff from uWebSockets.js v17.4.0 (commit 116a85f9668dcb03e7799db06dc8275bc43c0963)
 * to this file.
 *
 * Authored by Alex Hultman, 2018-2019.
 * Intellectual property of third-party.

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at

 *     http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OUROBORUS_HTTPRESPONSEDATA_H
#define OUROBORUS_HTTPRESPONSEDATA_H

/* This data belongs to the HttpResponse */

#include "HttpParser.h"
#include "AsyncSocketData.h"

#include "f2/function2.hpp"

namespace Ouroborus {

template <bool SSL>
struct HttpResponseData : AsyncSocketData<SSL>, HttpParser {
    template <bool> friend struct HttpResponse;
    template <bool> friend struct HttpContext;
private:
    /* Bits of status */
    enum {
        HTTP_STATUS_CALLED = 1, // used
        HTTP_WRITE_CALLED = 2, // used
        HTTP_END_CALLED = 4, // used
        HTTP_RESPONSE_PENDING = 8, // used
        HTTP_ENDED_STREAM_OUT = 16 // not used
    };

    /* Per socket event handlers */
    fu2::unique_function<bool(int)> onWritable;
    fu2::unique_function<void()> onAborted;
    fu2::unique_function<void(std::string_view, bool)> inStream; // onData
    /* Outgoing offset */
    int offset = 0;

    /* Current state (content-length sent, status sent, write called, etc */
    int state = 0;
};

}

#endif // OUROBORUS_HTTPRESPONSEDATA_H
