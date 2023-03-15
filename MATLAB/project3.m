% Initial setup
fs = 44100;
T = 3; % duration in seconds
N = T*fs; % total number of samp;es
x = zeros(N, 1); % silence

exciterLength = fs * 0.01; % duration of exciter in samples
exciterPeriod = fs * 0.5; % exciter hits every ___ samples
exciterADSR = ADSR(1, exciterLength - 2, 0, 1);

adsrPlot = x; 

for n = 1:N
    if mod(n, exciterPeriod) <= exciterLength
        x(n) = 2 * rand(1, 1) - 1;
    end
    
    if mod(n, exciterPeriod) == 1
        exciterADSR.noteOn();
    elseif mod(n, exciterPeriod) == exciterLength
        exciterADSR.noteOff();
    end
    
    adsrSample = exciterADSR.getSample();
    x(n) = x(n) * adsrSample;
    adsrPlot(n) = adsrSample;
        
end

%% LOAD loop filter coeffs and exciter signal from file
loopFilter = LoopFilter("loopfilter_exciter.mat"); % loop filter
%load("loopfilter_exciter.mat", 'exciter'); % exciter signal
[exciter, fs] = audioread("a4 exciter - 4.6k cut by 18db.wav");
% save it in x
exciterLength = fs * 0.01; % duration of exciter in samples
exciterPeriod = fs * 0.5; % exciter hits every ___ samples

for n = 1:N
    if mod(n, exciterPeriod) + 1 < size(exciter, 1)
        x(n) = exciter(mod(n, exciterPeriod) + 1);
    end
end

%% process


y = x;
lpf_fc = 10000;
lpf = LB_LPF(lpf_fc, 0.707, fs);
noteFreq = 220;
note2Freq = 220 * 9 / 8;
totalDelayTime = 1 / note2Freq; % in seconds 
totalDelaySamples = totalDelayTime * fs;
feedbackGain = 0.98;
delay = DelayLine(totalDelaySamples, ceil(totalDelaySamples) + 5, 0); % just delay, no fb
fbToDelay = 0; % sample to be added to input and fed back to delay

exciterADSR.noteOn();
for n = 1:N
    y(n) = delay.processSample(x(n) + fbToDelay);
    
    %filteredOutput = 0.5 * (y(n) + y_z1); % 2-sample avg filter
    filteredOutput = loopFilter.processSample(y(n));
    fbToDelay = filteredOutput * feedbackGain;
      
end

% later step?: use bidirectional delay line to simulate both directions
    % of traveling wave?

n = 1:N;

%% PLOT
% time domain
figure('Name', 'Time Domain');

subplot(2, 1, 1);
plot(n, x);
title("Input signal");
ylim([-1 1]);
xlim([0 exciterPeriod]);

subplot(2, 1, 2);
plot(n, y);
title("Output signal");
ylim([-1 1]);
xlim([0 exciterPeriod]);

% frequency domain
% numPoints = 4096;
% freq = 1:numPoints/2;
% 
% figure('Name', 'Frequency Domain');
% subplot(2, 1, 1);
% fftx = fft(x, numPoints);
% semilogx(freq, abs(fftx(1:numPoints/2)));
% title("Input signal");
% xlim([0 numPoints/2]);
% 
% subplot(2, 1, 2);
% ffty = fft(y, numPoints);
% semilogx(freq, abs(ffty(1:numPoints/2)));
% title("Output signal");
% xlim([0 numPoints/2]);

%% Play

outputGain = 1;
sound = audioplayer(outputGain * y, fs);
play(sound);