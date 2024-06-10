// SPDX-License-Identifier: (LGPL-2.1 OR BSD-2-Clause)
/* Copyright (c) 2021 Sartura
 * Based on minimal.c by Facebook */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/resource.h>
#include <bpf/libbpf.h>
#include "sampled_fentry_100.skel.h"

static int libbpf_print_fn(enum libbpf_print_level level, const char *format, va_list args)
{
	return vfprintf(stderr, format, args);
}


static volatile sig_atomic_t stop;

void sig_int(int signo)
{
	stop = 1;
	printf(" ]");
}

int main(int argc, char **argv)
{
	struct sampled_fentry_100_bpf *skel;
	int err;

	char* type = "sampled_fentry_100";
	if(argc > 1){
		type = argv[1];
		printf("Type: %s\n", type);
	}


	/* Set up libbpf errors and debug info callback */
	//libbpf_set_print(libbpf_print_fn);

	/* Open load and verify BPF application */
	skel = sampled_fentry_100_bpf__open_and_load();
	if (!skel) {
		fprintf(stderr, "Failed to open BPF skeleton\n");
		return 1;
	}

	/* Attach tracepoint handler */
	err = sampled_fentry_100_bpf__attach(skel);
	if (err) {
		fprintf(stderr, "Failed to attach BPF skeleton\n");
		goto cleanup;
	}

	if (signal(SIGINT, sig_int) == SIG_ERR) {
		fprintf(stderr, "can't set signal handler: %s\n", strerror(errno));
		goto cleanup;
	}

	printf("Successfully started! Please run `sudo cat /sys/kernel/debug/tracing/trace_pipe` "
	       "to see output of the BPF programs.\n");

	uint64_t counts = 0;
	int count_idx = 0; 
	uint64_t acc = 0;
	int acc_idx = 1;

	int t = 0;

	uint64_t zero = 0;


	printf("[ ");

	while (!stop) {
		//fprintf(stderr, ".");
		
		int ret = bpf_map__lookup_elem(skel->maps.gro_map, &acc_idx, sizeof(int), &acc, sizeof(uint64_t), 0);
                if(ret){
			printf("Error1: %d\n", ret);
			continue;
		}
		ret = bpf_map__lookup_elem(skel->maps.gro_map, &count_idx, sizeof(int), &counts, sizeof(uint64_t), 0);
		if(ret){
			printf("Error2: %d\n", ret);
			continue;
		}
                
		ret = bpf_map__update_elem(skel->maps.gro_map, &count_idx, sizeof(int), &zero, sizeof(uint64_t), 0);
		if(ret){
			printf("Error3: %d\n", ret);
			continue;
		}
		
		//printf("Counts: %lu %lu\n",counts, acc);
		
		//printf("%f pkts per GRO\n",(counts) ?(float)acc / (float)counts
		//				   : 0);
		printf("{ \"type\" : \"%s\", \"t\" : %d, \"pkts\" : %f }, \n",type, t, (counts) ?(float)acc / (float)counts
						   : 0);
		t++;
		sleep(1);
	}

cleanup:
	sampled_fentry_100_bpf__destroy(skel);
	return -err;
}
