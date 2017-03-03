#ifndef POSEIDON_PARSER_H
#define POSEIDON_PARSER_H

typedef struct _tuple{
    char* _key;
    int _value;
};

typedef struct  _tvector{
    int _size;
    struct _tuple* _vector;
};

struct _tvector config_vector;

void parse_config_file(char* file);

struct _tvector* _init_tvector(int size);
void _delete_tvector(struct _tvector* vector);
void _add_tuple(struct _tvector* vector);

struct _tvector _get_vector();
int _get_value(struct _tvector* vector, char* key);

#endif //POSEIDON_PARSER_H
