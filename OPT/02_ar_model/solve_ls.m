function [x] = solve_ls(A, b)
% function [x] = solve_ls(A, b)
%
% implements A\b using QR decomposition.
%
% INPUT: 
% A: an M-by-N matrix. It is assumed that that M>=N and 
% that A has full rank (=N). 
%
% b: an M-by-1 vector
%
% OUTPUT: 
% x: an N-by-1 vector, solving Ax=b in LSQ sense.

% discard the code from here and implement the functionality:
[Q,R] = qr(A,0);
x = zeros(size(R,1),1);
r = Q' * b;
counter = 1;
for y = length(x):-1:1
    if counter > 1   
        tmp = 0;
        for i = 1:(counter-1)            
             tmp = tmp + R(y,y+i)*x(y+i);  
        end
        x(y) = (r(y) -  tmp) / R(y,y);         
    else       
        x(y) = r(y) / R(y,y); 
    end
    counter = counter+1;
end