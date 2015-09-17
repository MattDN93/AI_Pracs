%--------------------------------------------------------------
%Solve a Pattern Recognition Problem with a Neural Network
%       Experiment 1: Feeding NN with matrices of footprints
%       Matthew de Neef | 212503024 | ENEL4AI
%       Input:  given matrices constructed via OpenCV application
%               these are padded to be 77x73 in size. 
%               column vectors are created for each animal with the entire matrix
%               This is used to train the NN
%               Once trained, simming the network allows it to learn and
%               solvwe.
%       Outputs: Output array, neural network object, various graphs
%               Run the M file to see how the training converges and sols
%               are found.
%       Prereq: 20 matrices of each animal <animalname>Matrix
%               Load these from the saved workspace given or
%               run the Read_in_Matrix.m script to generate a matrix from
%               text files.
%--------------------------------------------------------------

%---------------------define input matrices------------------------
%   pad arrays to same size by picking the biggest one (Elephant)
%   EXAMPLE: X is current pixels, E is expansion part to match Elephant
%   dims
%           X X X X E E E
%           X X X X E E E
%           X X X X E E E
%           E E E E E E E
%           E E E E E E E
%   OUTPUT: The above matrix expanded with padded background pixels
%--------------------------------------------------------------

largest_x = 0;
largest_y = 0;
[largest_x,largest_y] = size(elephantMatrix);

baboonMatrix(63:(63+(largest_x - 63)),1:(51+(largest_y - 51))) = 255;
buffaloMatrix(62:(62+(largest_x - 62)),1:(51+(largest_y - 51))) = 255;
wilddogMatrix(72:(72+(largest_x - 72)),1:(66+(largest_y - 66))) = 255;
cheetahMatrix(62:(62+(largest_x - 62)),1:(51+(largest_y - 51))) = 255;
clipspringerMatrix(62:(62+(largest_x - 62)),1:(58+(largest_y - 58))) = 255;
giraffeMatrix(62:(62+(largest_x - 62)),1:(51+(largest_y - 51))) = 255;
hippoMatrix(67:(67+(largest_x - 67)),1:(65+(largest_y - 65))) = 255;
hyenaMatrix(62:(62+(largest_x - 62)),1:(66+(largest_y - 66))) = 255;
impalaMatrix(63:(63+(largest_x - 63)),1:(51+(largest_y - 51))) = 255;
kuduMatrix(62:(62+(largest_x - 62)),1:(51+(largest_y - 51))) = 255;
leopardMatrix(62:(62+(largest_x - 62)),1:(51+(largest_y - 51))) = 255;
lionMatrix(62:(62+(largest_x - 62)),1:(63+(largest_y - 63))) = 255;
meerkatMatrix(62:(62+(largest_x - 62)),1:(51+(largest_y - 51))) = 255;
rhinoMatrix(65:(65+(largest_x - 65)),1:(56+(largest_y - 56))) = 255;
sableMatrix(62:(62+(largest_x - 62)),1:(51+(largest_y - 51))) = 255;
springbokMatrix(63:(63+(largest_x - 63)),1:(51+(largest_y - 51))) = 255;
warthogMatrix(62:(62+(largest_x - 62)),1:(54+(largest_y - 54))) = 255;

%convert all 255's to 1's
baboonMatrix(baboonMatrix == 255) = 1;
buffaloMatrix(buffaloMatrix == 255) = 1;
wilddogMatrix(wilddogMatrix == 255) = 1;
cheetahMatrix(cheetahMatrix == 255) = 1;
clipspringerMatrix(clipspringerMatrix == 255) = 1;
giraffeMatrix(giraffeMatrix == 255) = 1;
hippoMatrix(hippoMatrix == 255) = 1;
hyenaMatrix(hyenaMatrix == 255) = 1;
impalaMatrix(impalaMatrix == 255) = 1;
kuduMatrix(kuduMatrix == 255) = 1;
leopardMatrix(leopardMatrix == 255) = 1;
lionMatrix(lionMatrix == 255) = 1;
meerkatMatrix(meerkatMatrix == 255) = 1;
rhinoMatrix(rhinoMatrix == 255) = 1;
sableMatrix(sableMatrix == 255) = 1;
springbokMatrix(springbokMatrix == 255) = 1;
warthogMatrix(warthogMatrix == 255) = 1;

%----------------convert matrix rows into column vectors-----------
% PURPOSE:  Each matrix is converted to a long column vector for the NN
%           This allows each animal to occupy one column
% INPUT:    None.
% OUTPUT:   The matrix is converted to column vector and stored as such
%--------------------------------------------------------------
 
elephantVector = reshape(elephantMatrix.',[],1);
baboonVector = reshape(baboonMatrix.',[],1);
buffaloVector = reshape(baboonMatrix.',[],1);
wilddogVector = reshape(wilddogMatrix.',[],1);
cheetahVector = reshape(cheetahMatrix.',[],1);
clipspringerVector = reshape(clipspringerMatrix.',[],1);
giraffeVector = reshape(giraffeMatrix.',[],1);
hippoVector = reshape(hippoMatrix.',[],1);
hyenaVector = reshape(hyenaMatrix.',[],1);
impalaVector = reshape(impalaMatrix.',[],1);
kuduVector = reshape(kuduMatrix.',[],1);
leopardVector = reshape(leopardMatrix.',[],1);
lionVector = reshape(lionMatrix.',[],1);
meerkatVector = reshape(meerkatMatrix.',[],1);
rhinoVector = reshape(rhinoMatrix.',[],1);
sableVector = reshape(sableMatrix.',[],1);
springbokVector = reshape(springbokMatrix.',[],1);
warthogVector = reshape(warthogMatrix.',[],1);

% elephantVector = cell(77,1);
% for i = 1:77
%    elephantVector(i,1) = {elephantMatrix(i,:)};
% end
% 
% springbokVector = cell(77,1);
% for i = 1:77
%    springbokVector(i,1) = {springbokMatrix(i,:)};
% end
% 
% baboonVector = cell(77,1);
% for i = 1:77
%    baboonVector(i,1) = {baboonMatrix(i,:)};
% end
% 
% wilddogVector = cell(77,1);
% for i = 1:77
%    wilddogVector(i,1) = {wilddogMatrix(i,:)};
% end

%-------------------define targets----------------------------
%   PURPOSE:    The targets define which row holds the definition
%               of the "correct" animal. Each column represents one
%               choice of animal from 20 possible.
%               e.g. if column 1, row 1 = "1" the animal is a baboon etc
%   INPUT:      None.
%   OUTPUT:     20x20 matrix of possible animals. Each column has a unique
%               1 value per animal
%               outputs should match target after simming
%--------------------------------------------------------------
elephant_t = zeros(18,1); elephant_t(1) = 1;
baboon_t = zeros(18,1); baboon_t(2) = 1;
wilddog_t = zeros(18,1); wilddog_t(3) = 1;
buffalo_t = zeros(18,1); buffalo_t(4) = 1;
cheetah_t = zeros(18,1); cheetah_t(5) = 1;
clipspringer_t = zeros(18,1); clipspringer_t(6) = 1;
giraffe_t = zeros(18,1); giraffe_t(7) = 1;
hippo_t = zeros(18,1); hippo_t(8) = 1;
hyena_t = zeros(18,1); hyena_t(9) = 1;
impala_t = zeros(18,1); impala_t(10) = 1;
kudu_t = zeros(18,1); kudu_t(11) = 1;
leopard_t = zeros(18,1); leopard_t(12) = 1;
lion_t = zeros(18,1); lion_t(13) = 1;
meerkat_t = zeros(18,1); meerkat_t(14) = 1;
rhino_t = zeros(18,1); rhino_t(15) = 1;
sable_t = zeros(18,1); sable_t(16) = 1;
warthog_t = zeros(18,1); warthog_t(17) = 1;
springbok_t = zeros(18,1); springbok_t(18) = 1;
%springbok_t = zeros(20,1); springbok_t(19) = 1;
%springbok_t = zeros(20,1); springbok_t(20) = 1;

%---------------------create training and target sets------------------
%   PURPOSE: Training sets contain the 20 column vectors of matrices of
%            Each animal. This allows the NN to learn
%
%   is    [eleph] [baboon] [wilddog] [springbok] etc
%   if el    1        2       3        4         is=1
%   
%   INPUT:  20x20 matrix of each animal's matrix
%   OUTPUT: 2x sets of training and target sets
%
%--------------------------------------------------------------
training_set = [elephantVector baboonVector buffaloVector wilddogVector...
                cheetahVector clipspringerVector giraffeVector hippoVector ...
                hyenaVector impalaVector kuduVector leopardVector ...
                lionVector meerkatVector rhinoVector sableVector ...
                warthogVector springbokVector];
            
target_set = [elephant_t baboon_t buffalo_t wilddog_t ...
              cheetah_t clipspringer_t giraffe_t hippo_t ...
              hyena_t impala_t kudu_t leopard_t...
              lion_t meerkat_t rhino_t sable_t ...
              warthog_t springbok_t             ];

inputs = training_set;
targets = target_set;

%---------Create a Pattern Recognition Network-----------------
%format: input layer hiddenlayer[layer1 layer2] outputlayer
%follow rules given in brief
%using dimensions of elephant as max size
%--------------------------------------------------------------
hiddenLayerSize = [52 35];
net = patternnet(hiddenLayerSize);

%----- Set up Division of Data for Training, Validation, Testing---
%90% training 5% validation and 5% testing
%--------------------------------------------------------------
net.divideParam.trainRatio = 100/100;
net.divideParam.valRatio = 0/100;
net.divideParam.testRatio = 0/100;


%----------------Train the Network---------------------------------
%   NOTE:   Several training methods are shown, each with pros and cons
%           as listed,default will be trainscg if all left commented out
%           uncomment one to use. trainscg is default
%           The very slow convgce simply stop at 2000 epochs

%net.trainFcn = 'traincgp';         %fast conv <100 epochs, results ~80% accurate
%net.trainFcn = 'trainscg';         %slow conv >1000 epochs, results ~99% accurate
%net.trainFcn = 'trainrp';          %v.fast conv <50 epochs, results ~11% accurate
net.trainFcn = 'traincgb';         %fast conv <400 epochs, results ~95% accurate
%net.trainFcn = 'traincgf';         %hit epoch limit (2k), results ~95% accurate
%net.trainFcn = 'trainoss';         %hit epoch limit (2k), results ~95% accurate
%net.trainFcn = 'trainlm';          %errors, uses too much memory   
%net.trainFcn = 'trainbfg';         %errors, uses too much memory
%net.trainFcn = 'traingdx';         %hit epoch limit (2k), results ~95% accurate

net.trainParam.epochs = 2000;
[net,tr] = train(net,inputs,targets);


%----------------Test the Network---------------------------------
outputs = net(inputs);
errors = gsubtract(targets,outputs);
performance = perform(net,targets,outputs)

%----------------View the Network---------------------------------
%   Allows to view a pictorial representation of the network
%--------------------------------------------------------------
view(net)

%--------------------Plots------------------------------------
%   Shows metrics as required by the prac brief.
%--------------------------------------------------------------
 figure, plotperform(tr)
 figure, plottrainstate(tr)
 figure, plotconfusion(targets,outputs)
 figure, ploterrhist(errors)
 figure, plotroc(targets,outputs)