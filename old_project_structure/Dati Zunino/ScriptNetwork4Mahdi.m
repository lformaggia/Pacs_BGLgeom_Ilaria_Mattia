clear all;
close all;


%Defining \Omega, the tissue domain  % note: estremi = extremes

estremi = [0 550; 0 520; 0 230];  %rat98
 
%estremi = [0 1000; 0 1000; 0 200];  %rat93a

%estremi = [0 370; 0 250; 0 200];  %rat93b


% Unit of measure: the data are measured in \mu m. I want to transform the
% domain in a non dimensional domain. So I choose a characteristic
% dimension d = 50 \mu m


const = 50; 

estremi = estremi/const;






%Defining the .pts file on which I want to write my network 

file_ptsP='rat98P9lug2.pts';  % P = perfusion problem
file_ptsD='rat98D9lug2.pts';  % D = drug problem





%Defining the .txt file the I want to read (from Secomb's website!!)



%file_txt='./Filetxt/rattum98_0.txt';

file_txt='./Filetxt/rat98p.txt';

%file_txt='./Filetxt/rattm93b.txt';

%file_txt='rattm93a.txt';







move=0;   % dummy variable --> move=1 if I want to randomly pertube the network. Otherwise move =0.

sigmax=0.9;   % Parameters that quantify the magnitude of the movement of the network
sigmay=0.9;
sigmaz=0.9;

%constructing the network

[Lrete]= Network4Mahdi(file_txt, file_ptsP, file_ptsD, estremi, const, move, sigmax, sigmay, sigmaz);


%plot

hold on;
ylim= [0.0: 0.01: estremi(2,2)];
xlim = [0.0 :0.01: estremi(1,2)];
hold on;
plot(xlim, estremi(2,2), 'm-'); hold on

