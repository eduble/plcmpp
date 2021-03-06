/*******************************************************************************
 * Copyright (c) 2014 Etienne Dublé, Martin Kirchgessner, Vincent Leroy, Alexandre Termier, CNRS and Université Joseph Fourier.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * or see the LICENSE.txt file joined with this program.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/


#pragma once

#include <io/PatternsCollector.hpp>

namespace io {

class StdOutCollector
    : public PatternsCollector
{

protected:
    int64_t collected;
    int64_t collectedLength;

public:
    StdOutCollector();
    void collect(int32_t support, vector<int32_t>* pattern) override;
    int64_t close() override;
    int32_t getAveragePatternLength() override;

};

}
