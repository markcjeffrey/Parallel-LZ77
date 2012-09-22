#ifndef _TEST_H
#define _TEST_H

#include "Base.h"
#include "parallel.h"
#include "utils.h"
#include "gettime.h"
#include "stringGen.h"

inline int get_file_size(char * path) {
	struct stat info;
	stat(path, &info);
	return info.st_size;
}


inline void generateText(int *a, int n, int sigma)  {
	srand(time(NULL));
	for (int i = 0; i < n; i++)
		a[i] = rand() % sigma + 1;
}

inline void Usage(char *program) {
	printf("Usage: %s [options]\n", program);
	printf("-p <num>\tNumber of processors to use\n");
	printf("-d <num>\t2^n of character will be processed\n");
	printf("-r <num>\tGenerete random string with the specified alphabet size\n");
	printf("-i <file>\tInput file name\n");
	printf("-o <file>\tOutput file name\n");
	printf("-f <file>\tChoose different algorithm for LPF\n");
	printf("-h \t\tDisplay this help\n");
}

inline int test_main(int argc, char *argv[], char * algoname, std::pair<int *, int> lz77(int *s, int n)) {
	int opt;
	int p = 1, d = -1, n = -1;
	int sigma = -1;
	char path[1025] = {};
	int t = 0;

	while ((opt = getopt(argc, argv, "p:d:r:i:o:f:t:")) != -1) {
		switch (opt) {
			case 'p': {
				p = atoi(optarg);
				break;
			}
			case 'd': {
				d = atoi(optarg);
				n = 1 << d;
				break;
			}
			case 'r': {
				sigma = atoi(optarg);
				break;
			}
			case 'i': {
				strncpy(path, optarg, 1024);
				 if (dup2(open(optarg, O_RDONLY), STDIN_FILENO) < 0) {
				 	perror("Input file error");
				 	exit(EXIT_FAILURE);
				}
				break;
			}
			case 'o': {
				if (dup2(open(optarg, O_CREAT | O_WRONLY, 0644), STDOUT_FILENO) < 0) {
					perror("Output file error");
					exit(EXIT_FAILURE);
				} else {
					//TODO
				}
				break;
			}
			case 'f': {	//DON'T DO ANYTHING HERE!
				break;
			}

			default: {
				Usage(argv[0]);
				exit(1);
			}
		}
	}

	if (sigma < 0 && path[0] == 0) {
		perror("No input file specified / Random string genereted.");
		exit(1);
	}

	if (d < 0) {
		if (sigma < 0) {
	    	n = get_file_size(path);
	    } else {
			perror("Random data size not specified");
	    	exit(1);
		}
	}
	
#if defined(CILKP) 
	set_threads(p);
#endif
	printf("***************** TEST BEGIN *****************\n");

	int *s = newA(int,n+3);

	if (sigma >= 1) {
		printf(" * Data generated randomly with alphabet size: %d.\n", sigma);
		generateText(s, n, sigma);
	} else {
		printf(" * Data from file: %s\n", path);
		//int size =  get_file_size(path);

		seq<char> str = dataGen::readCharFile(path);
		intT size = str.size();
		if (n > size) {
			perror("The file is not as large as the size specified.");
			exit(1);
		}

		for (intT i = 0; i < n; i++) s[i] = (unsigned char) str[i];
		//s[n] = 0;
		str.del();


		//readText(a, n, path);
	}

	printf(" * Data size: %d\n", n);
	printf(" * Algorithm: %s\n", algoname);
#if defined(CILKP)
	printf(" * Threads num: %d\n", p);
#endif
	timer testTm;
	s[n] = s[n + 1] = s[n + 2] = 0;
	testTm.start();

	std::pair<int *, int> res = lz77(s, n);
	int maxoffset = n - res.first[res.second-1];
	for (int i = 0; i < res.second - 1; i++) {
		maxoffset = std::max(maxoffset, res.first[i+1] - res.first[i]);
	}
	printf(" * result: size = %d, max offset = %d\n", res.second, maxoffset);
	testTm.reportNext(" * Total time:");
	printf("***************** TEST ENDED *****************\n\n");
	free(s);
	//delete a;
	return 0;
}

#endif