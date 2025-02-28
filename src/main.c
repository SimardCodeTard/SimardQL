#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "main.h"

InputBuffer* new_input_buffer(void) {
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_size = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}

MetaCommandResult execute_meta_command(InputBuffer* input_buffer) {
    if(strcmp(input_buffer->buffer, ".exit") == 0) {
        close_input_buffer(input_buffer);
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
    return META_COMMAND_SUCCESS;
}

PrepareResult prepareStatement(InputBuffer* input_buffer, Statement* statement) {
    if(strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
    } else if (strncmp(input_buffer->buffer, "select", 6) == 0) {
        statement->type = STATEMENT_SELECT;
    } else {
        return PREPARE_UNRECOGNIZED_STATEMENT;
    }
    return PREPARE_SUCCESS;
}

void execute_statement(Statement* statement) {
    switch (statement -> type) {
        case STATEMENT_INSERT:
            printf("Insert statement \n");
            break;
        case STATEMENT_SELECT:
            printf("Select statement \n");
            break;
    }
}

void print_prompt(void) {
    printf("SimardQL > ");
}

void read_input(InputBuffer* input_buffer) {
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_size), stdin);

    if (bytes_read <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

int main(void) {
    char *version = getenv("VERSION");
    if (version == NULL) {
        printf("Error: VERSION environment variable not set, defaulting to 0.1\n");
        version = "0.1";
    }

    printf("Welcome to SimardQL version %s !\n", version);
    InputBuffer* input_buffer = new_input_buffer();
    while(true) {
        print_prompt();
        read_input(input_buffer);

        if(input_buffer->buffer[0] == '.') {
            switch (execute_meta_command(input_buffer)) {
                case META_COMMAND_SUCCESS:
                    continue;
                case(META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized meta command %s\n", input_buffer->buffer);
                    continue;
            }
        } else {
            Statement statement;
            switch (prepareStatement(input_buffer, &statement)) {
                case PREPARE_SUCCESS:
                    break;
                case (PREPARE_UNRECOGNIZED_STATEMENT):
                    printf("Unrecogniszed keyword at the start of %s\n", input_buffer->buffer);
                    continue;
            }
            execute_statement(&statement);
            printf("Executed.\n");
        }
    }
    return 0;
}
