%--------------------------------------------------------------
%MATLAB script to read input matrices from footprints
%   Matthew de Neef | 212503024 | ENEL4AI
%   FOR USE WITH:   AI_Test.exe output files
%   PURPOSE:        Takes the text file containing the matrix
%                   footprint data and centroid data, and
%                   strips out unnecessary delimeters
%   INPUT:          Text file with matrix and other data
%   OUTPUT:         1)ConnectedComponentData_IN.txt
%                   Text file with raw matrix
%                   2)<animal>Matrix matrix in the workspace
%   NOTE:           Use with Main NN program. 
%we remove spaces by delimiters and extra unnecessary stuff
%---------------------------------------------------------------
fin = fopen('ConnectedCompoData_Lion.txt','r');
fout = fopen('ConnectedCompo_IN.txt','wt');

%STRIPPING FUNCTION
%Purpose: Gets the file outputted by the connected compo app as-is and
%         strips out all unnecessary chars and text that isn't needed.
%Input:     The "ConnectedCompoData.txt" file without modification
%Output:    A new file with only the matrix, as well as InputMatrix var
%           for input into the neural network

 while ~feof(fin)
    s = fgetl(fin);                 %get each line in turn
    s = strrep(s, ', ', ',');       %remove all extra whitespace & random chars
    s = strrep(s, ',  ', ',');
    s = strrep(s, '[', '');
    s = strrep(s, ']', '');
    s = strrep(s, ';', char(10));   %add newline to ensure correct rows/columns
    if (strfind(s,'-')>0),break;end %removes extra text at end of file
    fprintf(fout,'%s',s);
    disp(s)
end

 fclose(fin);
 fclose(fout);

 %reads text matrix into matrix in MATLAB
lionMatrix = dlmread('ConnectedCompo_IN.txt',',',0,0);
%sable_Matrix = zeros(62, 5, 'uint8');
