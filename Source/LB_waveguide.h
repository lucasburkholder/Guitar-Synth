#ifndef LB_WAVEGUIDE_H
#define LB_WAVEGUIDE_H

#include <cmath>

namespace LB {

    class FractionalDelayLine {
    public:
        // Allows midi notes down to #31 (49hz) at 48khz sample rate.
        const static int maxDelay_samples = 1024; 

        FractionalDelayLine() = default;

        void init(float delayTime_samples) {
            cbufferSize = maxDelay_samples + lagrangePoints;
            writeIndex = 0;
            readIndex = float(writeIndex) - delayTime_samples;
            if (readIndex < 0)
                readIndex += cbufferSize;
        }

        double processSample(double xn) {
            cbufferSize = maxDelay_samples; // 
            int lagrangeStartIndex = floor(readIndex) - 1;
            lagrangeStartIndex = lagrangeStartIndex % cbufferSize;
            int lagrangeEndIndex = floor(readIndex) + 2;
            lagrangeEndIndex = lagrangeEndIndex % cbufferSize;
            double d[4]; //discrete values used to interpolate
            for (int i= 0; i < 4; i++) {

                d[i] = cbuffer[(lagrangeStartIndex + i) % cbufferSize];
            }

            //Read and interpolate output
            double yn = lagrangeInterpolate(readIndex - floor(readIndex), d);

            //Write input sample to buffer
            cbuffer[writeIndex] = xn;
        
            //Increment read/write indices
            writeIndex++;
            readIndex++;

            //Wrap if needed
            if (writeIndex == cbufferSize)
                writeIndex = 0;
            if (readIndex == cbufferSize)
                readIndex = 0;

            return yn;
        }

        void clearBuffer() {
            memset(&cbuffer, 0.0, sizeof(double) * maxDelay_samples);
        }

    private:
        

        double cbuffer[maxDelay_samples];
        int cbufferSize;
        int writeIndex;
        float readIndex;
        const int lagrangePoints = 4;

        double lagrangeInterpolate(float index, double discreteVals[4]) {

            // Take the 4-value vector discreteVals and return an interpolated value
            //   between the middle two discrete values.
            //   Index is a fraction (0-1) for how far along the x-axis from the first
            //   middle value to the second middle value to interpolate

            //Evaluate x (since l1, l2, l3, l4, and p are functions of x)
            float x = 2.f + index;

            // Find lagrange polynomials

            double l1 = (x - 2) * (x - 3) * (x - 4) / ((1 - 2) * (1 - 3) * (1 - 4));
            double l2 = (x - 1) * (x - 3) * (x - 4) / ((2 - 1) * (2 - 3) * (2 - 4));
            double l3 = (x - 1) * (x - 2) * (x - 4) / ((3 - 1) * (3 - 2) * (3 - 4));
            double l4 = (x - 1) * (x - 2) * (x - 3) / ((4 - 1) * (4 - 2) * (4 - 3));

            // Interpolate final polynomial

            double p = l1 * discreteVals[0] + l2 * discreteVals[1] + l3 * discreteVals[2] + l4 * discreteVals[3];
            return p;
        }
    };

    class LoopFilterGtr {
    public:
        LoopFilterGtr() = default;
        ~LoopFilterGtr() = default;

        void resetDelayRegisters() {
            x_z1 = y_z1 = 0.0;
        }

        double processSample(double xn) {
            double yn = a0 * xn + a1 * x_z1 - b1 * y_z1;

            // update delay registers
            y_z1 = yn;
            x_z1 = xn;

            return yn;
        }

    private:
        double a0 = 0.899, a1 = 0.0998, b1 = 0.003; //coefficients
        double x_z1 = 0.0, y_z1 = 0.0; //delay registers
    };
}

#endif