/*
# This file is Copyright 2002 Dean Hall.
# This file is part of the PyMite VM.
# This file is licensed under the MIT License.
# See the LICENSE file for details.
*/


#undef __FILE_ID__
#define __FILE_ID__ 0x0B


/**
 * \file
 * \brief List Object Type
 *
 * List object type operations.
 */


#include "pm.h"


PmReturn_t
list_append(pPmObj_t plist, pPmObj_t pobj)
{
    PmReturn_t retval;
    uint8_t objid;

    C_ASSERT(plist != C_NULL);
    C_ASSERT(pobj != C_NULL);

    /* If plist is not a list, raise a TypeError exception */
    if (OBJ_GET_TYPE(plist) != OBJ_TYPE_LST)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    /* Create new seglist if needed */
    if (((pPmList_t)plist)->length == 0)
    {
        retval = seglist_new(&((pPmList_t)plist)->val);
        PM_RETURN_IF_ERROR(retval);
    }

    /* Append object to list */
    heap_gcPushTempRoot((pPmObj_t)((pPmList_t)plist)->val, &objid);
    retval = seglist_appendItem(((pPmList_t)plist)->val, pobj);
    heap_gcPopTempRoot(objid);
    PM_RETURN_IF_ERROR(retval);

    /* Increment list length */
    ((pPmList_t)plist)->length++;

    return retval;
}


PmReturn_t
list_getItem(pPmObj_t plist, int16_t index, pPmObj_t *r_pobj)
{
    PmReturn_t retval;

    /* If it's not a list, raise TypeError */
    if (OBJ_GET_TYPE(plist) != OBJ_TYPE_LST)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    /* Adjust the index */
    if (index < 0)
    {
        index += ((pPmList_t)plist)->length;
    }

    /* Check the bounds of the index */
    if ((index < 0) || (index >= ((pPmList_t)plist)->length))
    {
        PM_RAISE(retval, PM_RET_EX_INDX);
        return retval;
    }

    /* Get item from seglist */
    retval = seglist_getItem(((pPmList_t)plist)->val, index, r_pobj);
    return retval;
}


PmReturn_t
list_insert(pPmObj_t plist, int16_t index, pPmObj_t pobj)
{
    PmReturn_t retval;
    int16_t len;
    uint8_t objid;

    C_ASSERT(plist != C_NULL);
    C_ASSERT(pobj != C_NULL);

    /* Raise a TypeError if plist is not a List */
    if (OBJ_GET_TYPE(plist) != OBJ_TYPE_LST)
    {
        retval = PM_RET_EX_TYPE;
        PM_RETURN_IF_ERROR(retval);
    }

    /* Adjust an out-of-bounds index value */
    len = ((pPmList_t)plist)->length;
    if (index < 0)
    {
        index += len;
    }
    if (index < 0)
    {
        index = 0;
    }
    if (index > len)
    {
        index = len;
    }

    /* Create new seglist if needed */
    if (((pPmList_t)plist)->length == 0)
    {
        retval = seglist_new(&((pPmList_t)plist)->val);
        PM_RETURN_IF_ERROR(retval);
    }

    /* Insert the item in the container */
    heap_gcPushTempRoot((pPmObj_t)((pPmList_t)plist)->val, &objid);
    retval = seglist_insertItem(((pPmList_t)plist)->val, pobj, index);
    heap_gcPopTempRoot(objid);
    PM_RETURN_IF_ERROR(retval);

    /* Increment list length */
    ((pPmList_t)plist)->length++;
    return retval;
}


PmReturn_t
list_new(pPmObj_t *r_pobj)
{
    PmReturn_t retval = PM_RET_OK;
    pPmList_t plist = C_NULL;

    /* Allocate a list */
    retval = heap_getChunk(sizeof(PmList_t), (uint8_t **)r_pobj);
    PM_RETURN_IF_ERROR(retval);

    /* Set list type, empty the contents */
    plist = (pPmList_t)*r_pobj;
    OBJ_SET_TYPE(plist, OBJ_TYPE_LST);
    plist->length = 0;
    plist->val = C_NULL;

    return retval;
}


PmReturn_t
list_copy(pPmObj_t pobj, pPmObj_t *r_pobj)
{
    return list_replicate(pobj, 1, r_pobj);
}


PmReturn_t
list_replicate(pPmObj_t psrclist, int16_t n, pPmObj_t *r_pnewlist)
{
    PmReturn_t retval = PM_RET_OK;
    int16_t i = 0;
    int16_t j = 0;
    int16_t length = 0;
    pPmObj_t pitem = C_NULL;
    uint8_t objid;

    C_ASSERT(psrclist != C_NULL);
    C_ASSERT(r_pnewlist != C_NULL);

    /* If first arg is not a list, raise TypeError */
    if (OBJ_GET_TYPE(psrclist) != OBJ_TYPE_LST)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }
    length = ((pPmList_t)psrclist)->length;

    /* Allocate new list */
    retval = list_new(r_pnewlist);
    PM_RETURN_IF_ERROR(retval);

    /* Copy srclist the designated number of times */
    for (i = n; i > 0; i--)
    {
        /* Iterate over the length of srclist */
        for (j = 0; j < length; j++)
        {
            retval = list_getItem(psrclist, j, &pitem);
            PM_RETURN_IF_ERROR(retval);
            heap_gcPushTempRoot(*r_pnewlist, &objid);
            retval = list_append(*r_pnewlist, pitem);
            heap_gcPopTempRoot(objid);
            PM_RETURN_IF_ERROR(retval);
        }
    }
    return retval;
}


PmReturn_t
list_setItem(pPmObj_t plist, int16_t index, pPmObj_t pobj)
{
    PmReturn_t retval;

    /* If it's not a list, raise TypeError */
    if (OBJ_GET_TYPE(plist) != OBJ_TYPE_LST)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    /* Adjust the index */
    if (index < 0)
    {
        index += ((pPmList_t)plist)->length;
    }

    /* Check the bounds of the index */
    if ((index < 0) || (index >= ((pPmList_t)plist)->length))
    {
        PM_RAISE(retval, PM_RET_EX_INDX);
        return retval;
    }

    /* Set the item */
    retval = seglist_setItem(((pPmList_t)plist)->val, pobj, index);
    return retval;
}


PmReturn_t
list_remove(pPmObj_t plist, pPmObj_t item)
{
    PmReturn_t retval = PM_RET_OK;
    uint16_t index;

    /* If it's not a list, raise TypeError */
    if (OBJ_GET_TYPE(plist) != OBJ_TYPE_LST)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    /* Locate the item to remove */
    retval = list_index(plist, item, &index);
    PM_RETURN_IF_ERROR(retval);

    /* Remove the item and decrement the list length */
    retval = seglist_removeItem(((pPmList_t)plist)->val, index);
    ((pPmList_t)plist)->length--;

    /* Unlink seglist if there are no contents */
    if (((pPmList_t)plist)->length == 0)
    {
        ((pPmList_t)plist)->val = C_NULL;
    }

    return retval;
}


PmReturn_t
list_index(pPmObj_t plist, pPmObj_t pitem, uint16_t *r_index)
{
    PmReturn_t retval = PM_RET_OK;
    pSeglist_t pseglist;
    pPmObj_t pobj;
    uint16_t index;

    /* If it's not a list, raise TypeError */
    if (OBJ_GET_TYPE(plist) != OBJ_TYPE_LST)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    /* Raise a ValueError if the list is empty */
    if (((pPmList_t)plist)->length == 0)
    {
        PM_RAISE(retval, PM_RET_EX_VAL);
        return retval;
    }

    pseglist = ((pPmList_t)plist)->val;

    /* Iterate over the list's contents */
    for (index = 0; index < pseglist->sl_length; index++)
    {
        retval = seglist_getItem(pseglist, index, &pobj);
        PM_RETURN_IF_ERROR(retval);

        /* If the list item matches the given item, return the index */
        if (obj_compare(pobj, pitem) == C_SAME)
        {
            *r_index = index;
            return PM_RET_OK;
        }
    }

    return PM_RET_EX_VAL;
}


PmReturn_t
list_delItem(pPmObj_t plist, int16_t index)
{
    PmReturn_t retval = PM_RET_OK;

    /* If it's not a list, raise TypeError */
    if (OBJ_GET_TYPE(plist) != OBJ_TYPE_LST)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    /* Adjust the index */
    if (index < 0)
    {
        index += ((pPmList_t)plist)->length;
    }

    /* Check the bounds of the index */
    if ((index < 0) || (index >= ((pPmList_t)plist)->length))
    {
        PM_RAISE(retval, PM_RET_EX_INDX);
        return retval;
    }

    /* Remove the item and decrement the list length */
    retval = seglist_removeItem(((pPmList_t)plist)->val, index);
    ((pPmList_t)plist)->length--;

    /* Unlink seglist if there are no contents */
    if (((pPmList_t)plist)->length == 0)
    {
        ((pPmList_t)plist)->val = C_NULL;
    }

    return retval;
}


#ifdef HAVE_PRINT
PmReturn_t
list_print(pPmObj_t plist)
{
    PmReturn_t retval = PM_RET_OK;
    int16_t index;
    pSeglist_t vals;
    pPmObj_t pobj1;

    C_ASSERT(plist != C_NULL);

    /* If it's not a list, raise TypeError */
    if (OBJ_GET_TYPE(plist) != OBJ_TYPE_LST)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    plat_putByte('[');

    vals = ((pPmList_t)plist)->val;

    /* Iterate over the list's contents */
    for (index = 0; index < ((pPmList_t)plist)->length; index++)
    {
        if (index != 0)
        {
            plat_putByte(',');
            plat_putByte(' ');
        }

        /* Print each item */
        retval = seglist_getItem(vals, index, &pobj1);
        PM_RETURN_IF_ERROR(retval);
        retval = obj_print(pobj1, C_FALSE, C_TRUE);
        PM_RETURN_IF_ERROR(retval);
    }

    return plat_putByte(']');
}
#endif /* HAVE_PRINT */


PmReturn_t
list_clear(pPmObj_t plist)
{
    PmReturn_t retval = PM_RET_OK;

    C_ASSERT(plist != C_NULL);

    /* Raise TypeError if arg is not a dict */
    if (OBJ_GET_TYPE(plist) != OBJ_TYPE_LST)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    /* Clear length and unlink seglist */
    ((pPmList_t)plist)->length = 0;
    ((pPmList_t)plist)->val = C_NULL;

    return retval;
}


#ifdef HAVE_SLICE
PmReturn_t 
list_slice(pPmObj_t plist, pPmObj_t pstart, pPmObj_t pend, pPmObj_t pstride, pPmObj_t *r_pslice)
{
    PmReturn_t retval = PM_RET_OK;

    int32_t start;
    int32_t end;
    int32_t stride;
    uint16_t len;
    pPmObj_t pslice;
    int16_t i;
    pPmObj_t pitem;
    uint8_t objid;

    len = ((pPmList_t)plist)->length;

    /* Handle the start index */
    if (OBJ_GET_TYPE(pstart) != OBJ_TYPE_INT)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    start = ((pPmInt_t)pstart)->val;

    if (start < 0)
    {
        start += len;

        if (start < 0)
        {
            start = 0;
        }
    }
    else if (start > len)
    {
        start = len;
    }

    /* Handle the end index */
    if (pend == PM_NONE)
    {
        end = len;
    }
    else
    {
        if (OBJ_GET_TYPE(pend) != OBJ_TYPE_INT)
        {
            PM_RAISE(retval, PM_RET_EX_TYPE);
            return retval;
        }

        end = ((pPmInt_t)pend)->val;

        if (end < 0)
        {
            end += len;

            if (end < 0)
            {
                end = 0;
            }
        }
        else if (end > len)
        {
            end = len;
        }
    }

    /* Handle the stride */
    if (OBJ_GET_TYPE(pstride) != OBJ_TYPE_INT)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    stride = ((pPmInt_t)pstride)-> val;

    /* Create the sequence to hold the slice */
    retval = list_new(&pslice);
    PM_RETURN_IF_ERROR(retval);

    /* Fill the slice */
    for (i = start; i < end; i += stride)
    {
        retval = list_getItem(plist, i, &pitem);
        PM_RETURN_IF_ERROR(retval);

        heap_gcPushTempRoot(pslice, &objid);
        retval = list_append(pslice, pitem);
        heap_gcPopTempRoot(objid);
        PM_RETURN_IF_ERROR(retval);
    }

    *r_pslice = pslice;
    return retval;
}
#endif /* HAVE_SLICE */
