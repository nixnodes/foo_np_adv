#pragma once

#include "stdafx.h"
#include "events.h"
#include "queue.h"
#include "writer_flags.h"

#include <atomic>
#include <fstream>

#define F_WRITER_APPEND		(uint32_t) 0x00000001
#define F_WRITER_ABORT		(uint32_t) 0x00000002

typedef struct write_job_s {
	pfc::string8 file;
	pfc::string8 data;
	uint8_t flags = 0;
	uint8_t encoding = 0;

	write_job_s(const pfc::string8 p_file, const pfc::string8 p_data, const uint8_t p_encoding, const uint8_t p_flags = 0) :
		file(p_file), data(p_data), encoding(p_encoding), flags(p_flags) {}
	write_job_s(const uint8_t p_flags) : flags(p_flags) {}
	write_job_s() {}

} write_job;

class CWriter {
public:
	CWriter() {
		t = new std::thread(std::bind(&CWriter::worker, this));
	}
	~CWriter()
	{
		p_Destroy = 1;
		cv_quit.notify_all();
		write_job j(F_WRITER_ABORT);
		q.push(j, true);
		t->join();
	}

	void QueueWrite(const write_job *j) { q.push(*j); }
	void QueueWriteAsync(const write_job *j, long long timeout);
	static void Write(write_job *j);

private:
	void worker();

	std::thread *t;
	Queue<write_job> q;

	static std::locale lmap[ENCODING_COUNT];

	static std::condition_variable cv_quit;
	static std::mutex cvq_mutex;
	static std::atomic<int> p_Destroy;
};

class IWriter {
public:
	static void Initialize() {
		if (m_Writer == nullptr) {
			m_Writer = new CWriter();
		}
	}

	static void Destroy() {
		if (m_Writer != nullptr) {
			delete m_Writer;
		}
	}

	static void Write(const write_job *j) {
		m_Writer->QueueWrite(j);
	}

	static void WriteAsync(const write_job *j, long long delay = 0) {
		m_Writer->QueueWriteAsync(j, delay);
	}
private:
	static CWriter *m_Writer;
};