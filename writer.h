#pragma once

#include "stdafx.h"
#include "events.h"
#include "queue.h"

#include <future>

#define F_WRITER_APPEND		(uint32_t) 0x00000001
#define F_WRITER_ABORT		(uint32_t) 0x00000002

typedef struct write_job_s {
	pfc::string8 file;
	pfc::string8 data;
	uint32_t flags = 0;

	write_job_s(pfc::string8 p_file,  pfc::string8 p_data, uint32_t p_flags) :
		file(p_file), data(p_data), flags(p_flags) {}
	write_job_s(pfc::string8 p_file, pfc::string8 p_data) :
		file(p_file), data(p_data) {}
	write_job_s(uint32_t p_flags) : flags(p_flags) {}
	write_job_s() {}

} write_job;



class CWriter {
public:
	CWriter() {
		t = new std::thread(std::bind(&CWriter::worker, this));
	}
	~CWriter()
	{
		pending_destroy = 1;
		cv_quit.notify_all();
		write_job j(F_WRITER_ABORT);
		q.push(j, true);
		t->join();
	}

	void QueueWrite(const write_job *j) { q.push(*j); }
	void QueueWriteAsync(const write_job *j, long long timeout);

private:
	void worker();
	void Write(write_job *j);

	std::thread *t;
	Queue<write_job> q;

	static std::condition_variable cv_quit;
	static std::mutex cvq_mutex;
	static std::atomic<int> pending_destroy;

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

	static void WriteAsync(const write_job *j, long long timeout = 0) {
		m_Writer->QueueWriteAsync(j, timeout);
	}
private:
	static CWriter *m_Writer;
};