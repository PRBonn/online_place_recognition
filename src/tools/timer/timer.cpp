/** online_place_recognition: a library for online matching of image sequences
** Copyright (c) 2017 O. Vysotska, C. Stachniss, University of Bonn
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
**/
#include "timer.h"
#include <iostream>

Timer::Timer() {}

/*
 * Start the timer, stop the current time.
 */
void Timer::start() { _start = std::chrono::high_resolution_clock::now(); }

/*
 * Stop the timer, store the current time.
 */
void Timer::stop() { _end = std::chrono::high_resolution_clock::now(); }

/*
 * Get total time used.
 */
std::chrono::seconds Timer::get_elapsed_s() {
  return std::chrono::duration_cast<std::chrono::seconds>(_end - _start);
}
std::chrono::milliseconds Timer::get_elapsed_ms() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start);
}

std::chrono::microseconds Timer::get_elapsed_micros() {
  return std::chrono::duration_cast<std::chrono::microseconds>(_end - _start);
}

std::chrono::nanoseconds Timer::get_elapsed_ns() {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(_end - _start);
}

void Timer::print_elapsed_time(TimeExt ext) {
  std::cerr << "TIMER::Elapsed time ";
  switch (ext) {
    case Sec: {
      std::cerr << get_elapsed_s().count() << " s.\n";
      break;
    }
    case MSec: {
      std::cerr << get_elapsed_ms().count() << " ms.\n";
      break;
    }
    case MicroSec: {
      std::cerr << get_elapsed_micros().count() << " micros.\n";
      break;
    }
    case NSec: {
      std::cerr << get_elapsed_ns().count() << " ns.\n";
      break;
    }
    default: { std::cerr << "\n Wrong time specification" << std::endl; }
  }
}
