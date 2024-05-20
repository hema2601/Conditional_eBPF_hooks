import json
import sys


json_dict = json.load(open(sys.argv[1], 'r', encoding="utf-8"))


vega_list = list()


for experiment in json_dict["Experiments"]:
    for run in experiment["Runs"]:
        for iperf_out in run["Outputs"]:
            #print(iperf_out)
            for interval in iperf_out["intervals"][2:-2]:
                #if interval["sum"]["start"] != 0:
                vega_list.append({  "name" : experiment["Parameters"]["BPF Program Name"],
                                    "throughput" : interval["sum"]["bits_per_second"]})


print(vega_list)
