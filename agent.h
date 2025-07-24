#ifndef AGENT_H
#define AGENT_H

#define IS_GENERATE 0
#define IS_CHAT 1

#include <jansson.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};

typedef struct AgentGenerateRequest {
    char *instructions;
    json_t *game_state;
    json_t *player;
    json_t *wants;
    char *snarky_commentary;
} gen_req;

char *generate(const json_t *data);
char *chat(const json_t *data);
json_t *invoke_agent(const json_t *data, int is_chat);

#endif