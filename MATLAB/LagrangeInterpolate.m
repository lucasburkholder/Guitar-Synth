function y = LagrangeInterpolate(index, discreteVals)
% Take the 4-value vector discreteVals and return an interpolated value
%   between the middle two discrete values.
%   Index is a fraction (0-1) for how far along the x-axis from the first
%   middle value to the second middle value to interpolate

    %% Evaluate x (since l1, l2, l3, l4, and p are functions of x)
    x = 2 + index;

    %% Find lagrange polynomials
    
    l1 = (x-2)*(x-3)*(x-4)/((1-2)*(1-3)*(1-4));
    l2 = (x-1)*(x-3)*(x-4)/((2-1)*(2-3)*(2-4));
    l3 = (x-1)*(x-2)*(x-4)/((3-1)*(3-2)*(3-4));
    l4 = (x-1)*(x-2)*(x-3)/((4-1)*(4-2)*(4-3));
    
    %% Interpolate final polynomial
    
    p = l1*discreteVals(1) + l2*discreteVals(2) + l3*discreteVals(3) + l4*discreteVals(4);
    
    %% Evaluate output value
    y = p;
end

%%