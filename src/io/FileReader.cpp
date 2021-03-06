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

#include <algorithm>
#include <fstream>
#include <iostream>
using namespace std;

#include <io/FileReader.hpp>
#include <util/Helpers.h>
using namespace util;

namespace io {

FileReader::CopyReader::CopyReader(
		Storage* storage) {
	_transactions_iterator = storage->getIterator();
}

FileReader::LineReader::LineReader(Storage *storage, string &path) {
	_storage = storage;
	_file = new std::ifstream(path);

	if (!_file->is_open())
	{
		cerr << "Could not open " << path << ". Aborting." << endl;
		abort();
	}
}

FileReader::LineReader::~LineReader()
{
	_file->close();
	delete _file;
}

int FileReader::LineReader::getTransactionSupport() {
	return 1;
}

enum READ_STATE {
	SPACES,
	INTEGER
};
void FileReader::LineReader::getTransactionBounds(int32_t* &begin, int32_t* &end) {
	string line;
	getline(*_file, line);
	enum READ_STATE state = SPACES;
	uint cur_int = 0;
	for (char c : line)
	{
		if (c == ' ')
		{
			if (state == SPACES)
				continue;
			else
			{
				_storage->push_back(cur_int);
				cur_int = 0;
				state = SPACES;
			}
		}
		else // a digit 0-9
		{
			if (state == SPACES)
				state = INTEGER;

			cur_int = (10*cur_int) + (c - '0');
		}
	}

	// do not forget the last one...
	if (state == INTEGER)
	{
		_storage->push_back(cur_int);
	}

	_storage->end_block(begin, end);
}

bool FileReader::LineReader::hasMoreTransactions() {
	return _file->good();
}

void FileReader::LineReader::prepareForNextTransaction() {
	_storage->start_block();
}

unique_ptr<FileReader::CopyReader> FileReader::getSavedTransactions() {
	return unique_ptr<FileReader::CopyReader>(
			new FileReader::CopyReader(_storage));
}

bool FileReader::hasNext() {
	return _reader->hasMoreTransactions();
}

FileReader::TransactionReader* FileReader::next() {
	_reader->prepareForNextTransaction();
	return _reader;
}

FileReader::FileReader(string& path) {
	_storage = new FileReader::Storage();
	_reader = new FileReader::LineReader(_storage, path);
}

FileReader::~FileReader() {
	delete _reader;
	delete _storage;
}

}
