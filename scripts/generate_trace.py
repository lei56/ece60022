import sys
import csv
import math
import argparse
import random

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', required=True)
    parser.add_argument('-u', default=200)
    parser.add_argument('-c', default=1)
    parser.add_argument('-m', default=3)
    parser.add_argument('-p', default=1)
    parser.add_argument('-q', default=100)
    parser.add_argument('-a', default=100)
    args = parser.parse_args()

    filename = args.f
    users = int(args.u)
    content_ids_per_user = int(args.c)
    max_content_ids_per_user = int(args.m)
    packets_per_user = int(args.p)
    max_packets_per_user = int(args.q)
    pct_active_users = int(args.a)

    with open(filename, 'w') as csvfile:
        for user in range(users):
            if (random.randrange(100) < pct_active_users):
                if (max_packets_per_user < packets_per_user):
                    max_packets_per_user = packets_per_user
                packets = random.randrange(packets_per_user, max_packets_per_user)
                
                if (max_content_ids_per_user < content_ids_per_user):
                    max_content_ids_per_user = content_ids_per_user
                content_ids = random.randrange(content_ids_per_user, max_content_ids_per_user)
                w_str = f'{user}, {user}, {packets}, {content_ids}\n'
                csvfile.write(w_str)

if __name__ == '__main__' : main()