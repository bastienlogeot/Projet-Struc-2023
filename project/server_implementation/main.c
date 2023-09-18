#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ngx_link_func_module.h>

int is_service_on = 0;
char *randomizator = "itfgiohtgiozerhjiopgzhrjioptgjheziopzioprgoipzerjutgiopujruzropiziozrrjgpiozrhjego";
const uint64_t mutliplication_seed = 0x5bd8e995;
const uint8_t padding_batch = 4;

/**
 * @brief Returns the constant multiplication_matrix.
 * 
 * @param ctx The context of the request, only used for logging and memory allocation.
 * @param K dimension of the matrix
 *
 * @return a K*K matrix
 *
 * @note You should use this function only once, and store its result. 
*/
int *get_multiplication_matrix(ngx_link_func_ctx_t *ctx, uint32_t K){
    uint32_t pos = 0;
    int *multiplication_matrix = ngx_link_func_palloc(ctx, K * K);
    for (size_t i = 0; i < K*K; i++){
        multiplication_matrix[i] = *(int *)(randomizator + i*sizeof(int));
        pos = (pos + 1) % strlen(randomizator);
    }
    return multiplication_matrix;
}

/**
 * @brief Process the request's body, and return a response. This is the function
 * you should implement.
 * 
 * @param ctx The context of the request, only used for logging and memory allocation.
 * @param body The request's body, as a string.
 * @param body_len The length of the request's body.
 * @param resp_len The length of the response.
 * 
 * @note You will do the required operations based on the request's body, and
 * return a response. BE CAREFUL, you MUST store the length of your response in `resp_len` 
 * before returning.
 * 
 * @note Also, this environment keeps you from doing classical `malloc` to allocate memory. Instead, use the function
 * `ngx_link_func_palloc(ctx, number_of_bytes)`. The advantage of this method is that your memory allocation is linked to
 * the request and everything is freed when the resquest finished. No need to worry about freeing memory :)
*/
static char* body_processing(ngx_link_func_ctx_t *ctx, char *body, size_t body_len, size_t *resp_len) {
    /**
     * TODO: Replace the example code below with your own code.
    */
    *resp_len = body_len;
    return body; 
}


void main_function(ngx_link_func_ctx_t *ctx) {

    // Retrieve request's body
    char *body = (char *)ctx->req_body;
    size_t body_len = ctx->req_body_len;

    // Process the request's body
    size_t resp_len = 0;
    char *resp = body_processing(ctx, body, body_len, &resp_len);
    // Warn user in case of error during processing
    if (resp == NULL){
        ngx_link_func_write_resp(
            ctx,
            500,
            "500 Internal Server Error",
            "text/plain",
            "Failed to parse request's body",
            sizeof("Failed to parse request's body") - 1
        );
        return;
    }
    // War user if he forgot to set the response's length
    if (resp_len == 0){
        ngx_link_func_write_resp(
            ctx,
            500,
            "500 Internal Server Error",
            "text/plain",
            "You forgot to set the response's length ! :angry:",
            sizeof("You forgot to set the response's length ! :angry:") - 1
        );
        return;
    }
    // Return the response
    ngx_link_func_write_resp(
        ctx,
        200,
        "200 OK",
        "text/plain",
        resp,
        resp_len
    );
}

/**
 * A function that is called when the application is started.
 * 
 * You shouldn't do anything here
*/
void ngx_link_func_init_cycle(ngx_link_func_cycle_t* cycle) {
    ngx_link_func_cyc_log(info, cycle, "%s", "Starting application, new logs !");
    is_service_on=1;
}

/**
 * A function that is called when the application is stopped.
 * 
 * You shouldn't do anything here
*/
void ngx_link_func_exit_cycle(ngx_link_func_cycle_t* cyc) {
    ngx_link_func_cyc_log(info, cyc, "%s\n", "Shutting down/reloading the Application");

    is_service_on = 0;
}