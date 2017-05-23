//============================================================================
// Name        : sliding_window.h
// Author      : Matous Dzivjak
// Version     : 0.1
// Copyright   : 2017, Matous Dzivjak
// Description : Sliding window utility for PSI semestral project
//============================================================================

#include <inttypes.h>
#include <vector>
#include <chrono>

#define MAX_RESEND_ATTEMPTS 8

using namespace std;

// Frame struct, holds info about data offset, send attempts, time of first send (I do not use this one), and if the data were succesfuly received
typedef struct frame_t {
	uint32_t data_offset;
	chrono::time_point<chrono::system_clock> time;
	bool ack;
	unsigned char attempts;
} frame_t;

// Window struct, circular queue with bonuses
typedef struct window_t {
	uint16_t size;
	vector<frame_t> window;
	uint16_t front, end, elements;
} window_t;

// Init sliding window with given size
window_t initWindow(int s);
// Add try to frame
bool windowAddTry(window_t* W, uint32_t noff);

// Is window full?
bool windowFull(window_t* W);

// Is window empty?
bool windowEmpty(window_t* W);

// Add frame to window
bool windowAdd(window_t* W, frame_t F);

// Acknowledge given data and slide window accordingly
void windowAck(window_t* W, uint32_t off);
