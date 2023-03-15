classdef DelayLine < handle
    %DELAYLINE Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        fb; % feedback parameter
        cb, cBufferSize, readIndex, writeIndex;
    end
    
    methods
        function obj = DelayLine(numSamples, maxDelaySamples, feedback)
            %DELAYLINE Construct an instance of this class
            
            % ASSERT that numSamples is greater than 4 (so we can still
            % interpolate)
            
            lagrangePoints = 4; % number of points to use for lagrange interpolation 
            
            obj.cb = zeros(1, maxDelaySamples + lagrangePoints);
            obj.cBufferSize = maxDelaySamples + lagrangePoints;
            obj.writeIndex = 1; % initialize write indexobj to 0
            
            obj.fb = feedback; % initialize feedback param
            
            obj.readIndex = obj.writeIndex - numSamples;
            if obj.readIndex < 1 % CHANGE TO 0 IN C++
                obj.readIndex = obj.readIndex + obj.cBufferSize;
            end
        end
        
        function yn = processSample(obj, xn)
            %yn = obj.cb(obj.readIndex); % read output from cbuffer
            
            lagrangeStartIndex = floor(obj.readIndex) - 1;
            lagrangeStartIndex = mod(lagrangeStartIndex, obj.cBufferSize) + 1;
            lagrangeEndIndex = floor(obj.readIndex) + 2;
            lagrangeEndIndex = mod(lagrangeEndIndex, obj.cBufferSize) + 1;
            if lagrangeStartIndex > obj.cBufferSize - 3
                discreteVals = [obj.cb(lagrangeStartIndex:obj.cBufferSize) obj.cb(1:lagrangeEndIndex)];
            else
                discreteVals = obj.cb(lagrangeStartIndex:lagrangeEndIndex);
            end
            yn = LagrangeInterpolate(obj.readIndex - floor(obj.readIndex), discreteVals);
            
            dn = xn + obj.fb * yn; % calculate input to delay line
            obj.cb(obj.writeIndex) = dn; % write to circular buffer
            
            % Increment write/read indices
            obj.writeIndex = obj.writeIndex + 1;
            obj.readIndex = obj.readIndex + 1;
            
            % Wrap if needed
            if obj.writeIndex == obj.cBufferSize + 1
                obj.writeIndex = 1; % CHANGE THIS TO 0 IN C++ CODE
            end
            if obj.readIndex == obj.cBufferSize + 1
                obj.readIndex = 1;
            end
                
        end
    end
end

