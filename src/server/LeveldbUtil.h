/*
 *
 * Copyright 2015, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef __DLDB_SRC_SERVER_LEVELDBUTIL_H__
#define __DLDB_SRC_SERVER_LEVELDBUTIL_H__

#include <assert.h>

#include <string>

#include "leveldb/db.h"

namespace dldb
{
	class LeveldbUtil
	{
		public:
			LeveldbUtil(const std::string& _dataDir)
					: dataDir(_dataDir),
					  db(NULL)
			{
			}

			~LeveldbUtil()
			{
				delete db;
				db = NULL;
			}

			bool connect()
			{
				leveldb::Options options;
				options.create_if_missing = true;
				leveldb::Status status = leveldb::DB::Open(options, dataDir, &db);

				return status.ok();
			}

			void insert(const std::string& key, const std::string& value,
						bool* ok, std::string* message)
			{
				assert(db != NULL);

				leveldb::Status status;
				status = db->Put(leveldb::WriteOptions(), key, value);

				*ok = status.ok();
				*message = status.ToString();
			}

			void del(const std::string& key, bool* ok, std::string* message)
			{
				assert(db != NULL);

				leveldb::Status status;
				status = db->Delete(leveldb::WriteOptions(), key);

				*ok = status.ok();
				*message = status.ToString();
			}

			std::string get(const std::string& key, bool* ok, std::string* message)
			{
				assert(db != NULL);

				leveldb::Status status;
				std::string value;
				status = db->Get(leveldb::ReadOptions(), key, &value);

				*ok = status.ok();
				*message = status.ToString();
			}

		private:
			// FOR NONCOPYABLE
			LeveldbUtil(const LeveldbUtil& );
			LeveldbUtil& operator = (const LeveldbUtil& );

		private:
			std::string dataDir;
			leveldb::DB *db;
	};
}


#endif  // __DLDB_SRC_SERVER_LEVELDBUTIL_H__
