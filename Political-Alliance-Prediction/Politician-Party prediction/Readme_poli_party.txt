Compiling Code: Before compiling code make sure all_edges.csv and topkhelper.csv files are present in same location as running the code.
		Compile using g++ compiler.
		g++ topk.cpp
		And run a.exe

Implementation Logic:

	For each party we are considering top K influential politicians from retweet network, currently influential nodes are found based on weighted indegree of politician.
	We calculate average sentiment of each politician participating from retweet network.
	Now, based on sentiment and interaction between the party we are predicting the party for current politician.
	Basically, for each party we are checking if current politician has ever interacted with the party or not, if yes then we find similarity based on sentiment between them.
	Thus, for a politician we will collect similarity with all influential politicians.
	Now, we check top T politicians who has maximum similarity with current politician and based on top T politicians we will check their parties and whichever party leads we will declare that current politician should belong to the party.

Hyperparameters are T and K and they are defined manually.
Min square error is considered as similarity measure.

Accuracy with T=7 and K=7 is: 90.5437

