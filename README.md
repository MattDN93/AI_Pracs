# AI_Pracs
A University AI Course, practicals, code and examples

##Practical Part 1
###1 Goal 
Given a binary image, write a code in C++ that uses Depth First Search algorithm to ﬁnd all the connected components in the image. For each connected component ﬁnd: 
* the centroid 
* the 7 invariant moments 
* the perimeter and the surface area 

Create a ﬁle in which each record containsdata collected from each image(number of connected components, data collected in (a), (b) and (c))
###2 The dataset 
The data set used consists of 20 folders each containing 3 images(60 images in total).

##Practical Part 2
###Goal
Artificial Neural Networks(ANN) is a popular and effective machine learning solution to many classification
problems. In this project, You will learn the different steps that are needed to design and use ANN. It
is then used to classify animals using their footprints. Data sets are provided.
###2 The data set
The data set used consists of images of footprints of animals (Baboon, Buffalo, Cheetar, Elephant, etc.).
###3 Design and Implement an Artificial Neural Network to classify Animals using their footprints.
*1. Matlab uses separate files for the input patterns and the output patterns. The inputs must be stored
to one file and outputs to another. Make sure that you don’t mess up the order of records across the
two files.
*2. Study the MATLAB Neural Networks toolbox, and split the file into training, validation and test
sets. Read the documentation and see how this should be done in Matlab and how the files should
be named.
*3. Design and implement a Multilayer perceptron and use it to classify animals represented by the files
given above. Two experiments must be carried out:
_ Experiment 1: inputs are the pixels of the image of an animal footprint._
_ Experiment 2: inputs are the features extracted from an image of animal footprint generated_
in practical 1.
###4 Performance evaluation
Compute how many of the animals are correctly classified and also how many are wrongly classified
for each class, in each of the experiments carried out in section 3. Build the confusion matrix, and
calculate True Positive and the False Positive rates [1]. Compare the classification performances of the
two experiments.
