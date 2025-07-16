#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};

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

int main(void) {
    CURL *curl;
    CURLcode res;

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        const char *json_data = "{\"model\":\"qwen3:8b\",\"prompt\":\"What is the capital of France?\",\"stream\":false}";

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:11434/api/generate");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("The curl result code is %d", res);
            printf("Received %zu bytes of data:\n", chunk.size);
            printf("%s\n", chunk.memory);
            json_error_t error;
            json_t *root = json_loads(chunk.memory, 0, &error);
            if (!root) {
                printf("I think I found an error! \n %s", error.text);
            } else {
                printf("No error, lets see what the response actually says.");
                json_t *model = json_object_get(root, "model");
                json_t *created_at = json_object_get(root, "created_at");
                json_t *response = json_object_get(root, "response");
                
                printf("LLM Output - Model: %s\n", json_string_value(model));
                printf("LLM Output - Created At: %s\n", json_string_value(created_at));
                printf("LLM Output - Response: %s\n", json_string_value(response));
                //} else 
                //    printf("Response field not found or not a string.\n");
                //}
                json_decref(root);
            }
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    free(chunk.memory);
    curl_global_cleanup();
    return 0;
}
