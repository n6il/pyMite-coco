/*
# This file is Copyright 2002 Dean Hall.
# This file is part of the PyMite VM.
# This file is licensed under the MIT License.
# See the LICENSE file for details.
*/


#ifndef __LIST_H__
#define __LIST_H__

/**
 * \file
 * \brief List Object Type
 *
 * List object type header.
 */

/**
 * List obj
 *
 * Mutable ordered sequence of objects.  Contains ptr to linked list of nodes.
 */
typedef struct PmList_s
{
    /** Object descriptor */
    PmObjDesc_t od;

    /** List length; number of objs linked */
    uint16_t length;

    /** Ptr to linked list of nodes */
    pSeglist_t val;
} PmList_t,
 *pPmList_t;


/**
 * Allocates a new List object.
 *
 * If there is not enough memory to allocate the List,
 * the return status will indicate an OutOfMemoryError
 * that must be passed up to the interpreter.
 * Otherwise, a ptr to the list is returned by reference
 * and the return status is OK.
 *
 * @param   r_pobj Return; addr of ptr to obj
 * @return  Return status
 */
PmReturn_t list_new(pPmObj_t *r_pobj);

/**
 * Gets the object in the list at the index.
 *
 * @param   plist Ptr to list obj
 * @param   index Index into list
 * @param   r_pobj Return by reference; ptr to item
 * @return  Return status
 */
PmReturn_t list_getItem(pPmObj_t plist, int16_t index, pPmObj_t *r_pobj);

/**
 * Sets the item in the list at the index.
 *
 * @param   plist Ptr to list
 * @param   index Index into list
 * @param   pobj Ptr to obj to put into list
 * @return  Return status
 */
PmReturn_t list_setItem(pPmObj_t plist, int16_t index, pPmObj_t pobj);

/**
 * Makes a copy of the given list.
 *
 * Allocate the necessary memory for root and nodes.
 * Duplicate ptrs to objs.
 *
 * @param   pobj Ptr to source list
 * @param   r_pobj Return; Addr of ptr to return obj
 * @return  Return status
 */
PmReturn_t list_copy(pPmObj_t pobj, pPmObj_t *r_pobj);

/**
 * Appends the given obj to the end of the given list.
 *
 * Allocate the memory for the node.
 * Do not copy obj, just reuse ptr.
 *
 * @param   plist Ptr to list
 * @param   pobj Ptr to item to append
 * @return  Return status
 */
PmReturn_t list_append(pPmObj_t plist, pPmObj_t pobj);

/**
 * Creates a new list with the contents of psrclist
 * copied pint number of times.
 * This implements the python code "[0,...] * N"
 * where the list can be any list and N is an integer.
 *
 * @param   psrclist The source list to replicate
 * @param   n The integer number of times to replicate it
 * @param   r_pnewlist Return; new list with its contents set.
 * @return  Return status
 */
PmReturn_t list_replicate(pPmObj_t psrclist, int16_t n, pPmObj_t *r_pnewlist);

/**
 * Inserts the object into the list at the desired index.
 *
 * @param   plist Ptr to list obj
 * @param   pobj Ptr to obj to insert
 * @param   index Index of where to insert obj
 * @return  Return status
 */
PmReturn_t list_insert(pPmObj_t plist, int16_t index, pPmObj_t pobj);

/**
 * Removes a given object from the list.
 *
 * @param   plist Ptr to list obj
 * @param   item Ptr to object to be removed
 * @return  Return status
 */
PmReturn_t list_remove(pPmObj_t plist, pPmObj_t item);

/**
 * Finds the first index of the item that matches pitem.
 * Returns an ValueError Exception if the item is not found.
 *
 * @param   plist Ptr to list obj
 * @param   pitem Ptr to object to be removed
 * @param   r_index Return by reference; ptr to index (C uint16)
 * @return  Return status
 */
PmReturn_t list_index(pPmObj_t plist, pPmObj_t pitem, uint16_t *r_index);

/**
 * Removes the item at the given index.
 * Raises a TypeError if the first argument is not a list.
 * Raises an IndexError if the index is out of bounds.
 *
 * @param   plist Ptr to list obj
 * @param   index Index of item to remove
 * @return  Return status
 */
PmReturn_t list_delItem(pPmObj_t plist, int16_t index);

#ifdef HAVE_PRINT
/**
 * Prints out a list. Uses obj_print() to print elements.
 *
 * @param pobj Object to print.
 * @return Return status
 */
PmReturn_t list_print(pPmObj_t pobj);
#endif /* HAVE_PRINT */

/**
 * Removes all items from the list and zeroes the length.
 *
 * @param plist List to clear
 * @return Return status
 */
PmReturn_t list_clear(pPmObj_t plist);

#ifdef HAVE_SLICE
/**
 * Creates a new list containing the described slice of the given list
 *
 * @param 	plist Ptr to list obj
 * @param	pstart Ptr to int object of slice start index
 * @param 	pend Ptr to int object of slice end index
 * @param 	pstride Ptr to int object of slice stride value
 * @param   r_pslice Reference of ptr to object containing resulting slice object
 * @return 	Return status
 */
PmReturn_t list_slice(pPmObj_t plist, pPmObj_t pstart, pPmObj_t pend, 
	                  pPmObj_t pstride, pPmObj_t *r_pslice);
#endif /* HAVE_SLICE */

#endif /* __LIST_H__ */
