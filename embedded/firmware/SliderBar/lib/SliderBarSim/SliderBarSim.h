#ifndef __SLIDERBARSIM_H__
#define __SLIDERBARSIM_H__

#include <stdint.h>

/*! \brief Simulates the SliderBar.
 *
 *  Simulates the SliderBar using a first order system with friction.
 */
class SliderBarSim
{
  public:
    SliderBarSim();
    ~SliderBarSim();

    /*! \brief Returns next SliderBar position.
     *
     *  Calculates and returns the next simulated SliderBar position.
     * 
     *  \param dt time since last update in seconds
     *  \param motorSpeed motor speed in percentage
     *  \return New position in percent
     */
    float update(float dt, float motorSpeed = 0.f);

  private:
    float position = 0.5f;

    float K = 1;
    float tau = 0.1; // s
};

#endif // __SLIDERBARSIM_H__
