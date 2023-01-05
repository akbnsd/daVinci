// This file is a part of  DaVinci
// Copyright (C) 2022  akshay bansod <aksh.bansod@yahoo.com>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.


#ifndef AUDIO_HANDLER_HPP
#define AUDIO_HANDLER_HPP


/**
 * @brief audioHandler interacts with the hardware to transact audio
 * 
 * The following are the specifications for audio data
*      bitrate          32 bit (signed int)
 *     sample rate      44100  (simplex)
 */
class audioHandler
{
public:

    // @brief callback function signature, called when recoded audio available
    typedef void (*recordCallback)(int* data, int size);

    audioHandler(recordCallback callback = 0);


    /**
     * @brief play the audio data
     * 
     * @param data signed int array containing audio data
     * @param size size of the array
     */
    void play(int *data, unsigned long size);

    /**
     * @brief toggle the current recording state
     * 
     * @param state force a recording state to override
     *          if state = -1      toggle
     *             state = 0       force stop
     *             state = 1       force start
     * 
     * forcing would be defined as setting current state irrespective of the current state.
     */
    void toggleRecording(int state = -1);

    /**
     * @brief sets record callback to recieve recorded data
     * 
     * on call to record(), periodic calls to passed callback is made passing the data as parameters
     * callback should process data quick and must be non holding
     * @param callback 
     */
    void setRecordCallback(recordCallback callback);

private:
    recordCallback m_callback = 0;

};
#endif //  AUDIO_HANDLER_HPP
