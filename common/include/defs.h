#pragma once
#ifndef _API_EXPORTS_
#define _API_EXPORTS_

#define _MPV mpv
#define BEGIN_NAMESPACE namespace _MPV{
#define END_NAMESPACE }
#define DEFAULT_NAME_FILE "test.txt"

#define API_DLL_EXPORT __declspec(dllexport)
#define API_DLL_IMPORT __declspec(dllimport)

#define API_STATIC_EXPORT
#define API_STATIC_IMPORT   API_DLL_IMPORT

#ifdef API_EXPORTS
#define API_EXPORT        API_DLL_EXPORT
#define API_EXPORT_STATIC API_STATIC_EXPORT
#else
#define API_EXPORT        API_DLL_IMPORT
#define API_EXPORT_STATIC API_STATIC_IMPORT
#endif
#endif
