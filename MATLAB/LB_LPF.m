classdef LB_LPF < handle
    %LB_LPF Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        fc, q, fs; % parameters
        xnm1, xnm2; % delay registers
        a0, a1, a2, b1, b2; % coefficients
    end
    
    methods
        function obj = LB_LPF(fc, q, fs)
            %LB_LPF Construct an instance of this class
            %   initialize coefficients and registers
            
            obj.fc = fc;
            obj.q = q;
            obj.fs = fs;
            
            [obj.xnm1, obj.xnm2] = deal(0);
            
            % calculate coefficients
            thetaC = 2*pi*fc/fs;
            d = 1/q;
            beta = 0.5 * (1 - (d / 2) * sin(thetaC)) / (1 + (d / 2) * sin(thetaC));
            gamma = (0.5 + beta) * cos(thetaC);
            
            obj.a0 = (0.5 + beta - gamma) / 2.0;
            obj.a1 = 0.5 + beta - gamma;
            obj.a2 = obj.a0;
            obj.b1 = -2 * gamma;
            obj.b2 = 2 * beta;
        end
        
        function yn = processSample(obj, xn)
            % process sample through the filter
            wn = xn - obj.b1 * obj.xnm1 - obj.b2 * obj.xnm2;

            yn = obj.a0 * wn + obj.a1 * obj.xnm1 + obj.a2 * obj.xnm2;

            obj.xnm2 = obj.xnm1;
            obj.xnm1 = wn;
        end
    end
end

