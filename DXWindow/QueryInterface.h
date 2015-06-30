/*
** Copyright (C) 2015 Austin Borger <aaborger@gmail.com>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
** API documentation is available here:
**		https://github.com/AustinBorger/DXAudio
*/

#pragma once

/* Handles the job of casting to a given interface in QueryInterface(). */
#define QUERY_INTERFACE_CAST(x)\
	if (riid == __uuidof(x)) {\
		x* pObj = static_cast<x*>(this);\
		pObj->AddRef();\
		*ppvObject = pObj;\
		return S_OK;\
	}

/* This should be called at the end of QueryInterface(). */
#define QUERY_INTERFACE_FAIL() *ppvObject = nullptr; return E_NOINTERFACE;