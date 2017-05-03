#ifndef INTERFACE_H
#define INTERFACE_H

#define SUCCESS 0
#define ID_ERROR 1 //for example
//etc...

/**
 * @fn     aquarium_load
 * @brief  loading an aquarium with path file
 * @param  path to config file
 * @return SUCCESS or error code otherwise (?)
 */
int aquarium_load(char * path);

/**
 * @fn     aquarium_add_view
 * @brief  adding a view given the id, coordinates, width and height
 * @param  id of the view (N, pouet, ...)
 * @param  x_coord x coordinate of the upper left corner (I suppose)
 * @param  y_coord y coordinate of the upper left corner
 * @param  view_width in px
 * @param  view_height in px
 * @return SUCCESS/... 
 */
int aquarium_add_view(char * id, int x_coord, int y_coord, int view_width, int view_height);

/**
 * @fn     aquarium_del_view
 * @brief  deleting a view
 * @param  id of the view
 * @return SUCCESS or error code (non-existing view...)
 */
int aquarium_del_view(char * id);

/**
 * @fn     aquarium_is_free_id 
 * @brief  is an id a view and free ?
 * @param  id
 * @return 1 if free, != else
 */
int aquarium_is_free_id(char * id);

/**
 * @fn
 * @brief  returns the first free id of an aquarium. Cf end of page 5 in the project description, "le controleur renvoie si possible un autre id, sinon une erreur"
 * @param  
 * @return id if exists, NULL if no free id
 */
char * aquarium_first_free();

#endif /*INTERFACE_H*/
