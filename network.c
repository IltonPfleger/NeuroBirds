typedef struct
{
	int size;
	int *sizes;
	double **B;
	double **O;
	double ***W;
} NN_Network;

double NN_ReLU(double x){
	return x > 0 ? x : 0;
}


NN_Network *NN_CreateNetwork(int size, int *sizes)
{
	NN_Network *network = (NN_Network *)malloc(sizeof(NN_Network));
	network->size = size;
	network->sizes = (int *)malloc(sizeof(int) * size);
	network->W = (double ***)malloc(sizeof(double **) * (size - 1));
	network->B = (double **)malloc(sizeof(double *) * (size - 1));
	network->O = (double **)malloc(sizeof(double *) * (size));
	network->O[0] = (double *)calloc(sizes[0], sizeof(double));

	for (int i = 0; i < size; i++)
	{
		network->sizes[i] = sizes[i];
	}

	for (int i = 0; i < size - 1; i++)
	{
		network->W[i] = (double **)malloc(sizeof(double *) * sizes[i + 1]);
		network->B[i] = (double *)calloc(sizes[i + 1], sizeof(double));
		network->O[i + 1] = (double *)calloc(sizes[i + 1], sizeof(double));

		for (int j = 0; j < sizes[i + 1]; j++)
		{
			network->W[i][j] = (double *)malloc(sizeof(double) * sizes[i]);
			for (int k = 0; k < sizes[i]; k++)
			{
				network->W[i][j][k] = ((double)rand()/(double)(RAND_MAX)) - 0.5;
			}
		}
	}
	return network;
}

void NN_FeedNetwork(NN_Network *network, double *inputs)
{
	for (int i = 0; i < network->sizes[0]; i++)
	{
		network->O[0][i] = inputs[i];
	}
	for (int i = 0; i < network->size - 1; i++)
	{
		for (int j = 0; j < network->sizes[i + 1]; j++)
		{
			network->O[i + 1][j] = 0;
			for (int k = 0; k < network->sizes[i]; k++)
			{
				network->O[i + 1][j] += network->W[i][j][k] * network->O[i][k];
			}
			network->O[i + 1][j] = NN_ReLU(network->O[i + 1][j]  + network->B[i][j]);
		}
	}
};

void NN_CopyNetwork(NN_Network n1, NN_Network *n2)
{
	for (int i = 0; i < n1.size - 1; i++)
	{
		for (int j = 0; j < n1.sizes[i + 1]; j++)
		{
			n2->B[i][j] = n1.B[i][j];
			for (int k = 0; k < n1.sizes[i]; k++)
			{
				n2->W[i][j][k] = n1.W[i][j][k];
			}
		}
	}
}

void NN_DestroyNetwork(NN_Network *network)
{
	free(network->O[0]);
	for (int i = 0; i < network->size - 1; i++)
	{
		for (int j = 0; j < network->sizes[i + 1]; j++)
		{
			free(network->W[i][j]);
		}
		free(network->W[i]);
		free(network->B[i]);
		free(network->O[i + 1]);
	}
	free(network);
};
