% Octave script to generate a tree network with the desired order of
% bifurcations. The network bifurcates the first time along the z 
% direction, the second on the x direction, and so on, proceeding 
% along the y direction. It also provide a random effect on the 
% final position of each node, but the first two, that are fixed.
% The network is contained in a 3D cube with unitary side.
% 
% Input parameters:
%	n = number of the order of bifurcation (at least 2 to obtain
%		a Y configuration.
%	re = random effect to apply to the nodes. When the new node 
%		is generated, a draw, multiplied by re, from a uniform 
%		between -1 and 1 is added to each coordinate. We perform
%		checks not to go out of bounds
%	out_filanem = name of the output file. Please, always put it 
%		in the data/ folder

close all
clear all
clc

% Number of order of bifurcation
n = 4;
% Setting random effect
re = 0.05;
% Output file name
out_filename = 'data/bifurcation_network.txt'

% Box lenght: unitary
L = 1;
% Initial point:
init = [0.5,0,0.5];

% y step size:
h_y = L/n;
% x step size:
h_x = L/(n-1);
% z step size
h_z = L/(n-2);

% Storing the points: (iteration 0)
points = init;

% First step: linear till the first next point (iteration 1)
new_point = [ init(1), init(2)+h_y, init(3)];
points = [points, new_point];   % I use the notation 'source' 'target'
% From the second iteration
for i = 2:n
    new_points = [];
    num_old_point_to_bifurcate = 2^(i-2);    
    for j = 0:num_old_point_to_bifurcate-1 %to set the indices in the proper way
    %If it is a an odd iteration, split on plane z
    if mod(i,2) == 0
        new_point1 = [points(end-j,4) - h_x/2 + re*(-1+2*rand(1)), ...
                      points(end-j,5) + h_y + re*(-1+2*rand(1)), ...
                      points(end-j,6) + re*(-1+2*rand(1))];
        new_point2 = [points(end-j,4) + h_x/2 + re*(-1+2*rand(1)), ...
                      points(end-j,5) + h_y + re*(-1+2*rand(1)), ...
                      points(end-j,6) + re*(-1+2*rand(1))];
    end
    % If it is an even iteration, split on plane x
    if mod(i,2) == 1
        new_point1 = [points(end-j,4) + re*(-1+2*rand(1)), ...
                      points(end-j,5) + h_y + re*(-1+2*rand(1)), ....
                      points(end-j,6) - h_z/2 + re*(-1+2*rand(1))];
        new_point2 = [points(end-j,4) + re*(-1+2*rand(1)), ...
                      points(end-j,5) + h_y + re*(-1+2*rand(1)), ...
                      points(end-j,6) + h_z/2 + re*(-1+2*rand(1))];
    end
    % Now we control wheter the new points are out of bound
    for k = 1:3
        if new_point1(1,k) - 1 < -1 %coordinate < 0
            new_point1(1,k) = 0;
        elseif new_point1(1,k) - 1 > 0 %coordinate > 1
            new_point1(1,k) = 1;
        end
        if new_point2(1,k) -1 < -1 %coordinate < 0
            new_point2(1,k) = 0;
        elseif new_point2(1,k) - 1 > 0 %coordinate > 1
            new_point2(1,k) = 1;            
        end
    end
    % Storing temporary points
    new_points = [new_points; points(end-j,4:6), new_point1];
    new_points = [new_points; points(end-j,4:6), new_point2];
    end
    % Storing all new points of this iteration
    points = [points; new_points];
end

% Plotting first point
plot3(points(1,1), points(1,2), points(1,3), '*', 'MarkerSize', 10)
% Plotting all other points
hold on
plot3(points(:,4), points(:,5), points(:,6), '*', 'MarkerSize', 10)
grid on
axis equal

% Printing the output on a txt file
dlmwrite(out_filename, points, 'delimiter', '\t', 'precision', '%.8f');
