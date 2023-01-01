#ifndef MIXER_HPP
#define MIXER_HPP


/**
 * @brief for mixing multiple audio waveforms
 * 
 * mixer uses opengl to mix audio in realtime. Each input stream is addressed by its stream.
 * operations such as @seek @record etc can be performed on streams. 
 * just generate or record multiple audio waveforms and call @play. */
class mixer
{

public:
    mixer();

    /**
     * @brief shift a particular stream by ms time 
     *  
     * @param stream index of the stream to be shifted
     * @param ms time in milliseconds. could be negative.
     * @return current timestamp in milliseconds
     */
    long seek(unsigned int stream, long ms);

    /**
     * @brief records audio data into particular stream
     * 
     * @param stream index of the stream to be recorded
     * call stopRec() to terminate stream recording
     */
    void record(int stream);

    /**
     * @brief enable or disable a particular stream
     * 
     * @param stream index of the stream to enable/disable
     * @param force pass true to force stream to enable ignoring the current state of stream
     * @return the state of the stream after toggle call
     * 
     * call to toggle() with force = false switches between enable/disable depending upon the
     * current state.
     */
    bool toggle(int stream, bool force = false);

    /**
     * @brief begin playback of superpose of active stream
     */
    void play();

    void pause();

    /**
     * @brief clear recorded stream data
     * 
     * @param stream index of the stream to be cleared
     */
    void clear(int stream);

    /**
     * @brief generate a function and write the wavefrom to the stream
     * 
     * @param stream index of stream to add sinewave to.
     * @param amplitude
     * @param w  2 * pi * f
     * @param phase phase offset of the initial value in rads
     * @param ms  duration of the overall stream to generate
     */
    void addFunction(int stream, int amplitude, int w, int phase, long ms);

    ~mixer();
};
#endif // MIXER_HPP
