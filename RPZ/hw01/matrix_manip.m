function output = matrix_manip(A,B)
% output = matrix_manip(A,B)
%
%   Perform example matrix manipulations.
%
%   Parameters:
%       A - matrix, arbitrary shape
%       B - matrix, <2 x n>
%
%   Return:
%       output.A_transpose
%       output.A_3rd_col
%       output.A_slice
%       output.A_gr_inc
%       output.C
%       output.A_weighted_col_sum
%       output.D
%       output.D_select
   
    % 1. Find the transpose of the matrix A:
    output.A_transpose = transpose(A)

    % 2. Select the third column of the matrix A:
    output.A_3rd_col = A(:,3)

    % 3. Select last two rows and last three columns of the matrix A and return the matrix in output.A_slice. 
    output.A_slice = A(end-1:end,end-2:end)

    % 4.Find all positions in A greater then 3 and increment them by 1 and add a column of ones to the matrix. Save the result to matrix A_gr_inc:
    output.A_gr_inc = [(A + (A > 3)) ones(size(A,1), 1)]

    % 5. Create matrix C such that Ci,j=∑nk=1A_gr_inci,k⋅A_gr_incTk,j and store it in output.C. 
    output.C = output.A_gr_inc * transpose(output.A_gr_inc)
 
    % 6. Compute ∑nc=1c⋅∑mr=1A_gr_incr,c:
    output.A_weighted_col_sum = sum(sum(output.A_gr_inc .* [1:size(output.A_gr_inc,2)]))
    
    % 7. Subtract a vector (4,6)T from all columns of matrix B. Save the result to matrix output.D.
    output.D = B - repmat(transpose([4 6]), 1, size(B,2))

    % 8. Select all vectors in the matrix D, which have greater euclidean distance than the average euclidean distance.
    output.D_select = output.D(:,find(sqrt(sum(output.D .^ 2))>mean(sqrt(sum(output.D .^ 2)))))

end
