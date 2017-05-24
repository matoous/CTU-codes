/*******************************************************************
project:         PSI semestral project by Matous Dzivjak
(C) Copyright:   2017 - 2017 by Matous Dzivjak
e-mail:          dzivjmat@fel.cvut.cz
license:         MIT
Feel free to copy as long as you wont use it for the homework
*******************************************************************/

#include "sliding_window.h"

// Init sliding window with given size
window_t initWindow(int s) {
	window_t w;
	w.size = s;
	w.window.resize(w.size);
	w.front = w.elements = 0;
	w.end = -1;
	return w;
}

// Add try to frame
bool windowAddTry(window_t* W, uint32_t noff) {
	int idx = (*W).front;
	for (int u = 0; u < (*W).elements; u++) {
		if ((*W).window[idx].data_offset == noff) {
			(*W).window[idx].attempts++;
			if ((*W).window[idx].attempts == MAX_RESEND_ATTEMPTS)
				return false;
		}
	}
	return true;
}

// Is window full?
bool windowFull(window_t* W) {
	return (*W).elements == (*W).size;
}

// Is window empty?
bool windowEmpty(window_t* W) {
	return (*W).elements == 0;
}

// Add frame to window
bool windowAdd(window_t* W, frame_t F) {
	if (windowFull(W))
		return false;
	else {
		(*W).end++;
		if ((*W).end == (*W).size)
			(*W).end = 0;
		(*W).window[(*W).end] = F;
		(*W).elements++;
	}
}

// Acknowledge given data and slide window accordingly
void windowAck(window_t* W, uint32_t off) {
	int idx = (*W).front;
	for (int u = 0; u < (*W).elements; u++) {
		if ((*W).window[idx].data_offset == off) {
			(*W).window[idx].ack = true;
			printf("Successfuly ACK frame with offset %u index %d in the window\n", off, idx);
			if (idx == (*W).front) {
				while ((*W).elements != 0 && (*W).window[(*W).front].ack) {
					(*W).front++;
					if ((*W).front == (*W).size)
						(*W).front = 0;
					(*W).elements--;
				}
			}
		}
		idx++;
		if (idx == (*W).size)
			idx = 0;
	}
}
