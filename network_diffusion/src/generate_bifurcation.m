% Number of order of bifurcation
n = 6;
% Setting random parameter
rp = 0.05;
% I don't randomize the y component, the direction along which i produce
% the bifurcations. This to not have to do out of bound controls

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
points = [points; new_point];
% From the second iteration
for i = 2:n
    new_points = [];
    num_old_point_to_bifurcate = 2^(i-2);    
    for j = 0:num_old_point_to_bifurcate-1 %to set the indices in the proper way
    %If it is a an odd iteration, split on plane z
    if mod(i,2) == 0
        new_point1 = [points(end-j,1) - h_x/2 + rp*(-1+2*rand(1)), ...
                      points(end-j,2) + h_y + rp*(-1+2*rand(1)), ...
                      points(end-j,3) + rp*(-1+2*rand(1))];
        new_point2 = [points(end-j,1) + h_x/2 + rp*(-1+2*rand(1)), ...
                      points(end-j,2) + h_y + rp*(-1+2*rand(1)), ...
                      points(end-j,3) + rp*(-1+2*rand(1))];
    end
    % If it is an even iteration, split on plane x
    if mod(i,2) == 1
        new_point1 = [points(end-j,1) + rp*(-1+2*rand(1)), ...
                      points(end-j,2) + h_y + rp*(-1+2*rand(1)), ....
                      points(end-j,3) - h_z/2 + rp*(-1+2*rand(1))];
        new_point2 = [points(end-j,1) + rp*(-1+2*rand(1)), ...
                      points(end-j,2) + h_y + rp*(-1+2*rand(1)), ...
                      points(end-j,3) + h_z/2 + rp*(-1+2*rand(1))];
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
    new_points = [new_points; new_point1; new_point2];
    end
    % Storing all new points of this iteration
    points = [points; new_points];
end

plot3(points(:,1), points(:,2), points(:,3), '*', 'MarkerSize', 10)
grid on
axis equal

% Printing the output on a txt file
dlmwrite('points.txt', points, 'delimiter', '\t', 'precision', '%.8f');