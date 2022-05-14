#include <stdio.h> /* io */
#include <unistd.h> /* syscalls */
#include <stdlib.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include <argp.h>

#include "can_interact.h" /* functionality containing both code to read and write from socket */

/**
  * @brief Example source code of writing to CAN using can_interact functionality
  */

#pragma GCC diagnostic ignored "-Wmissing-field-initializers" /* Below is some argp stuff. I'm ignoring some of the 'errors' */
#pragma GCC diagnostic push

    static char args_doc[] = "NET_DEVICE_NAME FRAME_ID DATA" ; /* description of non-option specified command line arguments */
    static char doc[] = "can_writer -- writes frame to CAN device" ; /* general program documentation */
    const char* argp_program_bug_address = "salih.msa@outlook.com" ;
    static struct argp_option options[] = {
        {0}
    } ;
    struct arguments {
        /**
          * @brief struct arguments - this structure is used to communicate with parse_opt (for it to store the values it parses within it)
          */
        char* args[3] ;  /* args for params */
    } ;

    /**
      * @brief parse_opt - deals with given arguments based on given argumentsK
      * @param int - int correlating to char storing argument key
      * @param char* - argument string associated with argument key
      * @param struct argp_state* - pointer to argp_state struct storing information about the state of the option parsing
      * @return error_t - number storing 0 upon successfully parsed values, non-zero exit code otherwise
      */
    static error_t parse_opt(int key, char *arg, struct argp_state* state)
    {
        struct arguments* arguments = (struct arguments*)state->input;

        switch (key)
        {
            case ARGP_KEY_ARG:
                if(state->arg_num >= 3)
                {
                    argp_usage(state);
                }
                arguments->args[state->arg_num] = arg;
                break;
            case ARGP_KEY_END:
                if (state->arg_num < 3)
                {
                    argp_usage(state);
                }
                break;
            default:
                return ARGP_ERR_UNKNOWN;
        }
        return 0 ;
    }

#pragma GCC diagnostic pop /* end of argp, so end of repressing weird messages */

int main(int argc, char** argv)
{
    /* Initialisation */
    struct arguments arguments;
    static struct argp argp = { /* argp - The ARGP structure itself */
        options, /* options */
        parse_opt, /* callback function to process args */
        args_doc, /* names of parameters */
        doc /* documentation containing general program description */
    } ;
    argp_parse(&argp, argc, argv, 0, 0, &arguments) ;

    const int s = can_socket_init(arguments.args[0]) ;

    /* Main functionality */
    int64_t value = (int64_t)strtol(arguments.args[2], NULL, 0) ;
    uint8_t bytes[8] = {0} ;
    const size_t bytes_used = number_to_hex_bytes(value, bytes, SIGNED_VAL, BIG_ENDIAN_VAL) ;

    int frame_id = strtol(arguments.args[1], NULL, 0) ;
    struct can_frame frame ; create_can_frame(frame_id, bytes, bytes_used, &frame) ;

    int status = send_can_frame(&frame, s) ;
    if(status != 0)
    {
        fprintf(stderr, "Error writing value %ld in frame with id 0x%x to %s\n", value, frame_id, arguments.args[0]) ;
    }
    else {
        fprintf(stdout, "Successfully sent value %ld in frame with id 0x%x to %s\n", value, frame_id, arguments.args[0]) ;
    }

    /* E(nd)O(f)P(rogram) */
    close(s) ;
    return 0 ;
}
