#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>

#include "agent.h"

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        fprintf(stderr, "Not enough memory!\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

// Invokes an LLM with Freeciv specific data structures for completion and value overrides
char *generate(const json_t *data) {
    if (!data) {
        printf("Null value passed to generate.\n");
        return;
    }

    json_t *agent_req = json_object();

}

// Invokes the LLM to just chit-chat about Freeciv. This can probably be hooked into the player chat.
char *chat(const json_t *data) {
    if (!data) {
        printf("Null value passed to chat.\n");
        return;
    }

    // Create prompt attribute with data
    json_t *agent_req = json_object();
    json_object_set(agent_req, "prompt", data);

    json_t *response_json = invoke_agent(agent_req, IS_CHAT);
    json_decref(agent_req);
    if (!response_json) {
        return NULL;
    }

    json_t *result = json_object_get(response_json, "result");
    if (!result) {
        return NULL;
    }
    char *ret = json_string_value(result);
    printf("LLM Output - Model: %s\n", ret);

    json_decref(response_json);
    return ret;
}

json_t *invoke_agent(const json_t *data, int is_chat) {
    if (!data) {
        printf("Null value passed to invoke_agent.\n");
        return;
    }
    
    CURL *curl;
    CURLcode res;

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    json_t *root = NULL;
    if (curl) {
        char *request_data = json_dumps(data, JSON_INDENT(2));
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        char full_url[100] = "http://localhost:8001/agent/";
        strcat(full_url, (is_chat == 1) ? "chat" : "generate");
        printf("fullurl %s\n", full_url);
        curl_easy_setopt(curl, CURLOPT_URL, full_url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_data);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("The curl result code is %d", res);
            //printf("Received %zu bytes of data:\n", chunk.size);
            //printf("%s\n", chunk.memory);
            json_error_t error;
            root = json_loads(chunk.memory, 0, &error);
            if (!root) {
                printf("I think I found an error! \n %s", error.text);
                return &error;
            }
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    free(chunk.memory);
    curl_global_cleanup();
    return root;
}
