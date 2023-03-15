classdef ADSR < handle
    %ADSR Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        attack, sustain, decay, release; % ADR values in samples
        currentValue;
        currentIncrement; % current increment per sample
        currentStage = 0; % 0 = idle, 1 = A, 2 = D, 3 = S, 4 = R
    end
    
    methods
        function obj = ADSR(a, d, s, r)
            %ADSR Construct an instance of this class
            %   Detailed explanation goes here
            obj.attack = a;
            obj.decay = d;
            obj.sustain = s;
            obj.release = r;
        end
        
        function noteOn(obj)
            obj.currentValue = 0;
            obj.currentStage = 1;
            obj.currentIncrement = 1 / obj.attack;
        end
        
        function noteOff(obj)
            obj.currentStage = 4;
        end
        
        % write noteOff function to trigger release phase
        
        function yn = getSample(obj)
            switch obj.currentStage
                case 0
                    obj.currentValue = 0;
                    obj.currentIncrement = 0;
                case 1
                    obj.currentValue = obj.currentValue + obj.currentIncrement;
                    if obj.currentValue >= 1
                        obj.currentIncrement = (obj.sustain - 1)/obj.decay;
                        obj.currentStage = 2;
                    end
                case 2
                    obj.currentValue = obj.currentValue + obj.currentIncrement;
                    if obj.currentValue <= obj.sustain
                        obj.currentValue = obj.sustain;
                        obj.currentStage = 3;
                        obj.currentIncrement = 0;
                    end
                case 3
                    obj.currentValue = obj.sustain;
                    obj.currentIncrement = 0;
                case 4
                    obj.currentValue = obj.currentValue + obj.currentIncrement;
                    obj.currentIncrement = -obj.sustain / obj.release;
                    
                    if obj.currentValue <= 0
                        obj.currentStage = 0;
                    end
            end
            
            
            yn = obj.currentValue;
        end
    end
end

