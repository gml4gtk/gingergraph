/*
 *   This file was automatically generated by version 1.7 of cextract.
 *   Manual editing not recommended.
 */

#ifndef __CEXTRACT__
#ifdef __STDC__

extern struct circular_list * newlist ( unsigned char value_type, int value_size );
extern int getsize ( struct circular_list *l );
extern int retrieve_pos ( struct circular_list *l );
extern unsigned char * retrieve ( int i, struct circular_list *l );
extern struct list_element * find_i_th ( int i, struct circular_list *l );
extern struct list_element * first_element ( struct circular_list *l );
extern void insert_last ( unsigned char *x, struct circular_list *l );
extern void init ( unsigned char *x, struct circular_list *l );
extern unsigned char * kopier ( unsigned char *src, struct circular_list *l );
extern void iterate ( funct f, struct circular_list *l );
extern unsigned char * get_last ( struct circular_list *l );

#else /* __STDC__ */

extern struct circular_list * newlist ( unsigned char value_type, int value_size );
extern int getsize ( struct circular_list *l );
extern int retrieve_pos ( struct circular_list *l );
extern unsigned char * retrieve ( int i, struct circular_list *l );
extern struct list_element * find_i_th ( int i, struct circular_list *l );
extern struct list_element * first_element ( struct circular_list *l );
extern void insert_last ( unsigned char *x, struct circular_list *l );
extern void init ( unsigned char *x, struct circular_list *l );
extern unsigned char * kopier ( unsigned char *src, struct circular_list *l );
extern void iterate ( funct f, struct circular_list *l );
extern unsigned char * get_last ( struct circular_list *l );

#endif /* __STDC__ */
#endif /* __CEXTRACT__ */
