#ifndef POSEIDON_PARSER_H
#define POSEIDON_PARSER_H

/**
 * Simple tuple, or MapEntry, associating a char* to an int
 */
struct _tuple{
    char* _key;
    int _value;
};

/**
 * Tuple vector, containing a vector of tuples (eh)
 */
struct  _tvector{
    int _size;
    int _ptr_current;
    struct _tuple *_vector;
};

/**
 * Global structure containing the parsed datas from the config file
 */
struct _tvector* config_vector;

/**
 * Main function to parse the config file
 * @param file the path to the file
 */
void parse_config_file(const char* file);

/**
 * Vector initialization
 * @param size
 * @return an allocated vector
 */
struct _tvector* _init_tvector(int size);

/**
 * To remove datas from a vector
 * @param vector
 */
void _delete_tvector(struct _tvector* vector);

/**
 * To add a tuple to the vector
 * @param vector
 * @param tuple
 */
void _add_tuple(struct _tvector* vector, char* key, int value);

/**
 * to create a tuple, with a pair key-value
 * @param key
 * @param value
 * @return
 */
struct _tuple _create_tuple(char* key, int value);

/**
 * To retrieve a value, according to its key (char*)
 * @param vector
 * @param key
 * @return
 */
int _get_value(struct _tvector* vector, char* key);

#endif //POSEIDON_PARSER_H
