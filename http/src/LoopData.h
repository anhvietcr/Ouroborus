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

#ifndef OUROBORUS_LOOPDATA_H
#define OUROBORUS_LOOPDATA_H

#include <thread>
#include <functional>
#include <vector>
#include <mutex>
#include <map>

#include "PerMessageDeflate.h"

#include "f2/function2.hpp"

namespace Ouroborus {

struct Loop;

struct alignas(16) LoopData {
    friend struct Loop;
private:
    std::mutex deferMutex;
    int currentDeferQueue = 0;
    std::vector<fu2::unique_function<void()>> deferQueues[2];

    /* Map from void ptr to handler */
    std::map<void *, fu2::unique_function<void(Loop *)>> postHandlers, preHandlers;

public:
    ~LoopData() {
        /* If we have had App.ws called with compression we need to clear this */
        if (zlibContext) {
            delete zlibContext;
            delete inflationStream;
            delete deflationStream;
        }
        delete [] corkBuffer;
    }

    /* Good 16k for SSL perf. */
    static const int CORK_BUFFER_SIZE = 16 * 1024;

    /* Cork data */
    char *corkBuffer = new char[CORK_BUFFER_SIZE];
    int corkOffset = 0;
    void *corkedSocket = nullptr;

    /* Per message deflate data */
    ZlibContext *zlibContext = nullptr;
    InflationStream *inflationStream = nullptr;
    DeflationStream *deflationStream = nullptr;
};

}

#endif // OUROBORUS_LOOPDATA_H
