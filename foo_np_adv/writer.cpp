#include "stdafx.h"
#include "writer.h"

#include <iostream>
#include <fstream>
#include <thread>

using namespace std;
using namespace std::chrono;

std::condition_variable CWriter::cv_quit;
std::mutex CWriter::cvq_mutex;
std::atomic<int> CWriter::p_Destroy{ 0 };

CWriter *IWriter::m_Writer = nullptr;

void CWriter::worker()
{
	console::info("NPA: writer thread starting");
	while (true)
	{
		write_job j = q.pop();
		if (j.flags & F_WRITER_ABORT) {
			break;
		}
		Write(&j);
	}
	console::info("NPA: writer thread exiting");
}

void CWriter::Write(write_job *j) {
	try {
		ofstream fs;
		unsigned int flags = ios::out;
		if (j->flags & F_WRITER_APPEND) {
			flags |= ios::app;
		}
		else {
			flags |= ios::trunc;
		}
		fs.open(j->file, flags);
		fs << j->data;
		fs.close();
	}
	catch (std::exception const & e) {
		console::complain("NPA: write failed", e);
	}
}

void CWriter::QueueWriteAsync(const write_job *j, long long delay) {
	std::thread([](const write_job j, CWriter *c, long long t) {
		std::unique_lock<std::mutex> lk(CWriter::cvq_mutex);
		CWriter::cv_quit.wait_for(lk, std::chrono::milliseconds(t), 
			[] {return CWriter::p_Destroy == 1; }
		);		
		c->q.push(j);
	}, *j, this, delay).detach();
	
}