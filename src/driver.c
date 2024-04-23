#include "driver.h"

// parameters
int timeslot_len = 100; // number of ns per timeslot
int curr_timeslot = 0;
int enable_edge_caching = 0;
int enable_transmitter_caching = 0;
int load = 100; // load value 1-100: frequency of packet generation = load%
int max_timeslot = 100;

// network
user_t * users[NUM_OF_USERS];
transmitter_t * transmitters[NUM_OF_TXERS];
edge_t * edges[NUM_OF_EDGES];
cdn_t * cdn;
links_t * links;

void work_per_timeslot() {
    printf("Simulation started\n");
    srand(time(NULL));

    while (1) {
/*---------------------------------------------------------------------------*/
                                  //CDN -- PROCESS
/*---------------------------------------------------------------------------*/
        cdn_process_packets(cdn, curr_timeslot);

/*---------------------------------------------------------------------------*/
                                  //EDGE -- PROCESS
/*---------------------------------------------------------------------------*/
        for (int i = 0; i < NUM_OF_EDGES; i++) {
            edge_t * edge = edges[i];
            edge_process_packets(edge, curr_timeslot);
        }

/*---------------------------------------------------------------------------*/
                                  //TXER -- PROCESS
/*---------------------------------------------------------------------------*/
        for (int i = 0; i < NUM_OF_TXERS; i++) {
            transmitter_t * txer = transmitters[i];
            transmitter_process_packets(txer, curr_timeslot);
        }
        
/*---------------------------------------------------------------------------*/
                                  //CDN -- RECEIVE
/*---------------------------------------------------------------------------*/
        cdn_receive_packets(cdn, links, curr_timeslot);

/*---------------------------------------------------------------------------*/
                                  //EDGE -- RECEIVE
/*---------------------------------------------------------------------------*/
        for (int i = 0; i < NUM_OF_EDGES; i++) {
            edge_t * edge = edges[i];
            edge_receive_packets(edge, links);
        }

/*---------------------------------------------------------------------------*/
                                  //TXER -- RECEIVE
/*---------------------------------------------------------------------------*/
        for (int i = 0; i < NUM_OF_TXERS; i++) {
            transmitter_t * txer = transmitters[i];
            transmitter_receive_packets(txer, links);
        }

/*---------------------------------------------------------------------------*/
                                  //USER -- RECEIVE
/*---------------------------------------------------------------------------*/
        for (int i = 0; i < NUM_OF_USERS; i++) {
            user_t * user = users[i];
            user_receive_packets(user, links, curr_timeslot);
        }

/*---------------------------------------------------------------------------*/
                                  //CDN -- SEND
/*---------------------------------------------------------------------------*/
        cdn_send_packets(cdn, links, curr_timeslot);

/*---------------------------------------------------------------------------*/
                                  //EDGE -- SEND
/*---------------------------------------------------------------------------*/
        for (int i = 0; i < NUM_OF_EDGES; i++) {
            edge_t * edge = edges[i];
            edge_send_packets(edge, links);
        }

/*---------------------------------------------------------------------------*/
                                  //TXER -- SEND
/*---------------------------------------------------------------------------*/
        for (int i = 0; i < NUM_OF_TXERS; i++) {
            transmitter_t * txer = transmitters[i];
            transmitter_send_packets(txer, links);
        }

/*---------------------------------------------------------------------------*/
                                  //USER -- SEND
/*---------------------------------------------------------------------------*/
        for (int i = 0; i < NUM_OF_USERS; i++) {
            user_t * user = users[i];
            user_send_packets(user, links, curr_timeslot, load);
        }

/*---------------------------------------------------------------------------*/
                                  //UPDATE PARAMETERS
/*---------------------------------------------------------------------------*/
        if (curr_timeslot >= max_timeslot) {
            return;
        }
        curr_timeslot++;
    }
}

arguments_t * process_args(int argc, char ** argv) {
    arguments_t * arguments = create_arguments();
    int opt;
    char out_suffix[4] = ".out";

    while ((opt = getopt(argc, argv, "f:a:b:c:x:y:z:m:e:t:")) != -1) {
        switch(opt) {
            case 'f':
                if (strlen(optarg) < 500) {
                    strcpy(arguments->filename, optarg);
                    strncpy(arguments->out_filename, arguments->filename, strlen(arguments->filename));
                    strncat(arguments->out_filename, out_suffix, 4);
                }
                break;
            case 'a':
                arguments->user_txer_delay = atoi(optarg);
                break;
            case 'b':
                arguments->txer_edge_delay = atoi(optarg);
                break;
            case 'c':
                arguments->edge_cdn_delay = atoi(optarg);
                break;
            case 'x':
                arguments->txer_cache_size = atoi(optarg);
                break;
            case 'y':
                arguments->edge_cache_size = atoi(optarg);
                break;
            case 'z':
                arguments->cdn_cache_size = atoi(optarg);
                break;
            case 'm':
                arguments->txer_memory_access_delay = atoi(optarg);
                arguments->edge_memory_access_delay = atoi(optarg);
                arguments->cdn_memory_access_delay = atoi(optarg);
                break;
            case 'e':
                enable_edge_caching = 0;
                printf("disabled edge caching\n");
                break;
            case 't':
                enable_transmitter_caching = 0;
                printf("disabled transmitter caching\n");
                break;
            default:
                printf("wrong command line argument\n");
                exit(1);
        }
    }

    return arguments;
}

void read_tracefile(char * filename) {
    FILE * fp;
    if (strcmp(filename, "")) {
        printf("opening tracefile %s\n", filename);
        fp = fopen(filename, "r");
        if (fp == NULL) {
            perror("open");
            exit(1);
        }
        int flow_id = -1;
        int src = -1;
        int flow_size = -1;
        int content_ids = -1;
        while (fscanf(fp, "%d,%d,%d,%d", &flow_id, &src, &flow_size, &content_ids) == 4) {
            flow_t * flow = create_flow(flow_id, flow_size, content_ids, src, 0);
            if (users[src]->flow != NULL) {
                free_flow(users[src]->flow);
            }
            users[src]->flow = flow;
        }
        printf("flows initialized\n");
        fclose(fp);
    }

    return;
}

void initialize_network(arguments_t * arguments) {
    // create users
    for (int i = 0; i < NUM_OF_USERS; i++) {
        users[i] = create_user(i);
    }
    // create transmitters
    for (int i = 0; i < NUM_OF_TXERS; i++) {
        transmitters[i] = create_transmitter(i, arguments->txer_cache_size, arguments->txer_memory_access_delay, enable_transmitter_caching);
    }
    // create edges
    for (int i = 0; i < NUM_OF_EDGES; i++) {
        edges[i] = create_edge(i, arguments->edge_cache_size, arguments->edge_memory_access_delay, enable_edge_caching);
    }
    // create cdn
    cdn = create_cdn(0, arguments->cdn_cache_size, arguments->cdn_memory_access_delay);
    // create links
    links = create_links();

    return;
}

void evaluate_network() {
    float avg_delay = 0;
    int active_users = 0;
    for (int i = 0; i < NUM_OF_USERS; i++) {
        user_t * user = users[i];
        
        if (user->pkts_received > 0) {
            avg_delay += user->avg_content_delay;
            active_users++;
        }
        
    }
    avg_delay /= active_users;
    avg_delay *= timeslot_len;
    printf("Average content delay: %0.0f ns\n", avg_delay);
}

void free_network() {
    // free users
    for (int i = 0; i < NUM_OF_USERS; i++) {
        free_user(users[i]);
    }
    // free transmitters
    for (int i = 0; i < NUM_OF_TXERS; i++) {
        free_transmitter(transmitters[i]);
    }
    // free edges
    for (int i = 0; i < NUM_OF_EDGES; i++) {
        free_edge(edges[i]);
    }
    // free cdn
    free_cdn(cdn);
    // create free links 
    free_links(links);

    return;
}

int main(int argc, char ** argv) {
    arguments_t * arguments = process_args(argc, argv);
    initialize_network(arguments);
    read_tracefile(arguments->filename);
    free_arguments(arguments);

    work_per_timeslot();

    evaluate_network();
    free_network();

    printf("finished execution\n");

    return 0;
}