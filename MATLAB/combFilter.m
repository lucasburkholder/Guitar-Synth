function y = combFilter(freqInterval)
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here
g1 = (0.5)^3;        % Some specific coefficients
g2 = (0.9)^5;
B = [1 0 0 g1];      % Feedforward coefficients, M1=3
A = [1 0 0 0 0 g2];  % Feedback coefficients, M2=5
N = 1000;            % Number of signal samples
x = rand(N,1);       % Random test input signal
y = filter(B,A,x);   % Matlab and Octave compatible


sampleDelay = 3;
B = [ 1 zeros(1, sampleDelay - 1) g1];
[h, w] = freqz(B, 1, 1024);
figure;
subplot(211);
semilogx((1:1024), abs(h));
subplot(212);
plot((1:1024), abs(h));
end

