#ifndef __SLIDERBAR_H__
#define __SLIDERBAR_H__

class SliderBar {
public:
    SliderBar();
    ~SliderBar();

    /**
     *  @brief Runs the main loop of the SliderBar
     *  It decodes the messages sent through USB, and moves the motor
     *  accordingly.
     *  
     *  @retval None
     */
    void run();

    //void receive
};

SliderBar* g_sliderbar_ptr = nullptr;

#endif // __SLIDERBAR_H__
