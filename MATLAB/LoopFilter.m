classdef LoopFilter < handle
    % Generates both loop filter and exciter signal 
    
    properties
        % Coefficients
        a0, a1, b0, b1;
        
        % delay registers
        x_z1, x_z2, y_z1, y_z2;
        
        % exciter signal
        exciter
    end
    
    methods
        function obj = LoopFilter(filename)
            %UNTITLED2 Construct an instance of this class
            %   Detailed explanation goes here
            load(filename, 'A', 'B');
            obj.a0 = A(1); 
            obj.a1 = A(2);
            obj.b0 = B(1);
            obj.b1 = B(2);
            
            % Initialize delay registers
            obj.x_z1 = 0;
            obj.x_z2 = 0;
            obj.y_z1 = 0;
            obj.y_z2 = 0;
           
            
        end
        
        function yn = processSample(obj, xn)
            % Diff equation: a(1)*y(n) = b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)
            %              - a(2)*y(n-1) - ... - a(na+1)*y(n-na)
            yn = obj.b0 * xn + obj.b1 * obj.x_z1 - obj.a1 * obj.y_z1;
            
            % update delay registers
            obj.x_z2 = obj.x_z1;
            obj.x_z1 = xn;
            obj.y_z2 = obj.y_z1;
            obj.y_z1 = yn;
        end
        
    end
end

