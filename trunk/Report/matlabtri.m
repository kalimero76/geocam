% This program can generate a delaunay triangulation and convert it to a
% format like that on the Manifold Page. With that, we could try and run
% flows on it or other things. This is for an unweighted delaunay
% triangulation. 

r = 10;
%global X, Y;
X = -10 + 20*rand(r,1);
Y = -10 + 20*rand(r,1);

TRI = delaunay(X,Y);
for i = 1:size(TRI,1)
    TRI(i,1:3) = sort(TRI(i,1:3));
end
TRI = sortrows(TRI)

filename = 'c:\Dev-Cpp\geocam\Triangulations\MATLABTRI2.txt';
filenamE = 'c:\Dev-Cpp\geocam\Triangulations\EdgeLengths.txt';
filenamD = 'c:\Dev-Cpp\geocam\Triangulations\Degrees.txt';
fid = fopen(filename, 'w');
fid2 = fopen(filenamE, 'w');
fid3 = fopen(filenamD, 'w');

%Produce the traingulation format

fprintf(fid,'blah blah blah =[');
for u = 1:size(TRI,1)
    fprintf(fid, '[');
    fprintf(fid, '%i', TRI(u,1));
    fprintf(fid, ',');
    fprintf(fid, '%i', TRI(u,2));
    fprintf(fid, ',');
    fprintf(fid, '%i', TRI(u,3));
    fprintf(fid, ']');
    if u ~= size(TRI,1)
        fprintf(fid,',');
    end
end
fprintf(fid, ']');

%Produce number of Degrees for Statistics

K = max(max(TRI));

for i = 1:K 
    sum = 0;
    for k = 1:size(TRI,1)
        B = TRI(k,1:3);
        if (i == B(1) || i == B(2) ||i == B(3))
            A = combntns((TRI(k,1:3)),2);
            for j = 1:3
                if (i ~= A(j,1) && i ~= A(j,2))
                    sum = [sum A(j,1) A(j,2)];
                end
            end
        end
    end
    sum = unique(sum);
    fprintf(fid3, '%2i ', size(sum,2) - 1);
end



%Produce edge lengths for Statistics

for i = 1:K
    len = 0;
    for j = i+1:K %so no duplicates
        for k = 1:size(TRI,1)
            A = combntns((TRI(k,1:3)),2);
            for h = 1:3
            if (i == A(h,1) && j == A(h,2))
                len = [len (sqrt((X(i) - X(j))^2 + (Y(i) - Y(j))^2))];
            end
            end
        end
    end
    len = unique(len);
    len = len(1,2:size(len,2));
    fprintf(fid2, '%.6G ', len);
end

