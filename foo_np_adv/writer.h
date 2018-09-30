#pragma once

#include "events.h"
#include "queue.h"
#include "writer_flags.h"

#include <atomic>
#include <locale>
#include <thread>
#include <fstream>

#define F_WRITER_APPEND		(uint32_t) 0x00000001
#define F_WRITER_ABORT		(uint32_t) 0x00000002

typedef struct write_job_s {
	pfc::string8 file;
	pfc::string8 data;
	uint8_t flags;
	uint8_t encoding = 0;

	write_job_s(const pfc::string8 p_file, const pfc::string8 p_data, const uint8_t p_encoding, const uint8_t p_flags = 0) :
		file(p_file), data(p_data), encoding(p_encoding), flags(p_flags) {}
	write_job_s(const uint8_t p_flags) : flags(p_flags) {}
} write_job;

class CWriter {
public:
	CWriter() {
		thread = new std::thread(std::bind(&CWriter::worker, this));
	}
	~CWriter()
	{
		try {
			queue.push(write_job(F_WRITER_ABORT), true);
			thread->join();
		}
		catch (std::exception const &) {
			// ignore
		}
	}

	void QueueWrite(const write_job &j);
	static void Write(const write_job &j);

private:
	void worker();

	std::thread *thread;
	Queue<write_job> queue;

	static std::locale lmap[ENCODING_COUNT];
};

class IWriter {
public:
	static void Initialize() {
		if (m_Writer == nullptr) {
			p_Destroy = false;
			m_Writer = new CWriter();
		}
	}

	static void Destroy() {
		if (m_Writer != nullptr) {
			p_Destroy = true;
			cv_quit.notify_all();
			delete m_Writer;
			m_Writer = nullptr;
		}
	}

	static void Write(const write_job &j) {
		m_Writer->QueueWrite(j);
	}

	static void WriteAsync(const write_job &j, const long long delay) {
		std::thread([](const write_job j, CWriter *c, const long long t) {
			std::unique_lock<std::mutex> lk(cvq_mutex);
			cv_quit.wait_for(lk, std::chrono::milliseconds(t),
				[] {return p_Destroy == 1; }
			);
			c->QueueWrite(j);
		}, j, m_Writer, delay).detach();
	}
private:
	static CWriter *m_Writer;
	static std::atomic<bool> p_Destroy;
	static std::condition_variable cv_quit;
	static std::mutex cvq_mutex;
};