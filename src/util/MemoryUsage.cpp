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

#include "util/MemoryUsage.hpp"
#include "Config.hpp"

#include <unistd.h>
#include <iostream>
#include <fstream>
using namespace std;

namespace util {
namespace MemoryUsage {

WatcherThread::WatcherThread() :
						PollableThread(Config::MEMORY_USAGE_CHECK_DELAY) {
	_kbytes_per_page = getpagesize() / 1024;
	cout << "kbytes_per_page = " << _kbytes_per_page << endl;
}

WatcherThread::~WatcherThread() {
}

void WatcherThread::onPoll(bool timeout) {
	printRSS();
}

void WatcherThread::onInit() {
	printRSS();
}

void WatcherThread::printRSS() {
	ifstream reader("/proc/self/statm");
	uint i;
	reader >> i;
	reader >> i; // second integer is the RSS (resident set size)
	cout << "Memory (RSS): " << i * _kbytes_per_page << "k" << endl;
}

long getPeakMemoryUsage() {
	static struct rusage rusage_holder;
	getrusage(RUSAGE_SELF, &rusage_holder);
	return rusage_holder.ru_maxrss;
}

}
}

