% Initial setup
fs = 44100;
T = 3; % duration in seconds
N = 10; % total number of samp;es
x = [1 zeros(1, N-1)]; % impulse input
y = zeros(1, N);
delaySamples = 5.3;
delay = DelayLine(delaySamples, floor(delaySamples + 5), 0);
fbToDelay = 0;
feedbackGain = 0.7;

for n = 1:N
    y(n) = delay.processSample(x(n));
    fbToDelay = y(n) * feedbackGain;
end

n = (1:N);
figure;
subplot(2, 1, 1);
plot(n, x);
ylim([-1 1]);
subplot(2, 1, 2);
plot(n, y);
ylim([-1 1]);