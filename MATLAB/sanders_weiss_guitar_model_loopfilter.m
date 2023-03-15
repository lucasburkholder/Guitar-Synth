%% MATLAB Code by Sanders & Weiss
%% design loop filter based on procedure in Karjalainen paper

[B, fs] = audioread('a4 exciter.wav');
noteFreq = 370;

B = B(:,1);
inputlength = size(B);
soundSample = B;

%differentiate B to smooth out signal
B = diff(B);


%[b f t] = specgram(B, 2048, fs, blackman(2048), 1024);
[b f t] =  spectrogram(B, blackman(2048), 1024, 2048, fs);
figure(1), subplot(311), plot(f,abs(b));
title('STFT of guitar sample');
xlabel('Frequency (Hz)');
ylabel('Magnitude');
axis([0 1e4 0 0.5]);

%fundamental freq
F=370;

%number of freqs to plot
Nf = 10;

%compute stft:
freqs = F:F:F*Nf;
%freqs = [F/3 F/2 freqs];
window = 1024;
overlap = window/2;  %50% overlap
%[b f t] = specgram(B, freqs, fs, blackman(window), overlap);
[b f t] = spectrogram(B, blackman(window), overlap, freqs, fs);
b=b';

%we're only interested in the magnitude spectrum:
b = abs(b);

figure(1), subplot(312), plot(t, b);
title('Guitar sample harmonics vs. time');
ylabel('Magnitude (log scale)');
xlabel('Time (samples)');

[rows cols] = size(b);

%remove first few samples of b - we're not interested in 
%components from the initial initial pluck 
deltah = b((10:length(b)),(1:cols));

[rows cols] = size(deltah);

for r = 10:rows
  for c = 1:cols
    deltah(r,c) = 20*log10(deltah(r,c));
  end
end


lines = zeros(rows,cols);
slopes = zeros(1,cols);

for c = 1:cols

  %fit to straight line:
  l = deltah(:,c);

  x = t(10:length(t));
  m = polyfit(l,x,1);

  lines(:,c) = m(1)*x + m(2);

  slopes(c) = m(1);
end;


firresp = zeros(1, length(slopes));

for h = 1:length(slopes) 
  Bk = slopes(h);
  
  %length of delay line
  L  = F/fs;
  
  %hop size used in stft
  H  = overlap/fs;

  firresp(h) = 10^(Bk*L/(20*H));
end



%assume harmonics after Nfth harmonic decay linearly
step=2*pi*F/fs;
first=2*pi*(F*(Nf+1))/fs;

i=first:step:pi;
%firfill=(.35*(pi-i));

%tweaked firfill to get reasonable attenuation at high frequencies
%(if it decays all the way to zero, the higher harmonics attenuate
%very rapidly)
firfill=.95:-.35/(length(i)-1):.6;

figure(1), subplot(313), stem(freqs,firresp)
title('Extrapolated FIR filter coefficients based on harmonic decay');
xlabel('Frequency (Hz)');
axis([0 freqs(length(freqs)) .9 1]);

[B, A] = invfre qz([firresp firfill], [2*pi*freqs/fs i], 1, 1)

%add a gain factor to keep fundamental freq (low freq) from attenuating 
%too rapidly we needed to tweak this filter by hand to get the best results
B= 1.105*B;

figure(2),freqz(B, A);
title('Loop filter response');


%% 

exciter = getexcitesignal(B, A, soundSample, 220, fs);
exciter = exciter(1:20000); % only save beginning of exciter signal

figure(3);
subplot(211);
plot([1:size(soundSample)], soundSample);
subplot(212);
plot([1:size(exciter)], exciter);

sound = audioplayer(exciter, fs);
play(sound);

% Save to file
save('loopfilter_exciter.mat', 'B', 'A', 'exciter');