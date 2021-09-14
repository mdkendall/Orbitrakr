/*
 *  Excerpt from Hamlib Interface API header: rig.h
 *  Original copyright statement follows:
 *
 *  Copyright (c) 2000-2003 by Frank Singleton
 *  Copyright (c) 2000-2012 by Stephane Fillod
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef HAMLIB_ERR_H
#define HAMLIB_ERR_H

/**
 * \brief Hamlib error codes
 * Error code definition that can be returned by the Hamlib functions.
 * Unless stated otherwise, Hamlib functions return the negative value
 * of rig_errcode_e definitions in case of error, or 0 when successful.
 */
enum rig_errcode_e {
    RIG_OK = 0,     /*!< No error, operation completed successfully */
    RIG_EINVAL,     /*!< invalid parameter */
    RIG_ECONF,      /*!< invalid configuration (serial,..) */
    RIG_ENOMEM,     /*!< memory shortage */
    RIG_ENIMPL,     /*!< function not implemented, but will be */
    RIG_ETIMEOUT,   /*!< communication timed out */
    RIG_EIO,        /*!< IO error, including open failed */
    RIG_EINTERNAL,  /*!< Internal Hamlib error, huh! */
    RIG_EPROTO,     /*!< Protocol error */
    RIG_ERJCTED,    /*!< Command rejected by the rig */
    RIG_ETRUNC,     /*!< Command performed, but arg truncated */
    RIG_ENAVAIL,    /*!< function not available */
    RIG_ENTARGET,   /*!< VFO not targetable */
    RIG_BUSERROR,   /*!< Error talking on the bus */
    RIG_BUSBUSY,    /*!< Collision on the bus */
    RIG_EARG,       /*!< NULL RIG handle or any invalid pointer parameter in get arg */
    RIG_EVFO,       /*!< Invalid VFO */
    RIG_EDOM        /*!< Argument out of domain of func */
};

#endif
