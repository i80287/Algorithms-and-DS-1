printf("calling sendto in %s:%u at %s\n", __FILE__, (uint32_t)__LINE__, FUNCTION_NAME_MACRO);
        printf("calling recv in %s:%u at %s\n", __FILE__, (uint32_t)__LINE__, FUNCTION_NAME_MACRO);
        printf("received message of type %s in %s:%u at %s\n", message_type_to_string(message.message_type), __FILE__, (uint32_t)__LINE__, FUNCTION_NAME_MACRO);