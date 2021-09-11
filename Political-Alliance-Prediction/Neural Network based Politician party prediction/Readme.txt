Google Colab Notebook Link:
https://colab.research.google.com/drive/11056s9dUXOa86URBHETlB4QWYm54GADF#scrollTo=qvWRDEBMV-w6

Data-Preparation Work:
Node Embedded data has been generated using DeepWalk technique on retweet network and It also contains politicians to each community's average sentiment.

Important points in python notebook file:
1. Upload node embedded data (train_data.csv file) generated in the notebook file section.
2. Import required modules i.e. nn (for neural network), torchvision (for datasets), numpy, pandas for working with CSV etc.
3. Read node embedded vectors using pandas and separate labels from it.
4. Now reshape the embedded vector according to the number of rows and number of columns in the input data.
5. Convert labels into numpy array and apply the OneHot-Encoding technique using the fit_transform method of OneHotEncoder.
6. Use TensorDataset for preparing the dataset.
7. Split the whole dataset into training and testing data based on the 80-20 rule with the help of DataLoader.
8. Shuffle these two datasets with batch size as 32, num_workers as 3 and pin_memory set to true.
9. Prepare a CNN model with following layers:
	a. layer1: Convolution2D with kernel size: 2*2, stride: 1*1, padding: 1*1 and out_channels:16
	   it returns matrix of [32, 16, 2, 44] size
	b. layer2: MaxPool2d with kernel size: 2*2, stride: 2*2
	   it returns matrix of [32, 16, 1, 22] size
	c. layer3: Convolution2D with kernel size: 2*2, stride: 1*1, padding: 1*1 and   out_channels:16
	   it returns matrix of [32, 16, 2, 23] size
	d. layer4: MaxPool2d with kernel size: 2*2, stride: 2*2
	   it converts the matrix into [32, 16, 1, 11] size
	e. Apply reshape for the required shape
	   it returns the matrix of [32, 176] size
	f. layer5: Linear transformation with num_classes as target class (i.e. 29 in our case)
	   it returns matrix of [32, 29] size
10. HyperParameter used:
	Adam Optimizer with learning rate as 0.001 and betas as (0.5, 0.999),
	num_epochs set to 100,
	Activation function: LeakyReLU with alpha parameter 0.2
11. Along with this, we have also used Softmax for rescaling with dimension factor as 1.
12. For calculating the loss, we have used the Cross entropy loss function. 
13. Run the model for defined number of epochs, and compute and loss, apply gradient descent and use the optimizer
14. Find the accuracy for the model against the actual label.
15. Print the accuracy for train and test data.	

Training Accuracy: 49.759
Testing Accuracy: 55.468
