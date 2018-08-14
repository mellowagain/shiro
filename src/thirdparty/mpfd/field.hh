#ifndef SHIRO_FIELD_HH
#define SHIRO_FIELD_HH

#include "exception.hh"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <sstream>

namespace MPFD {

    class Field {
    public:
        static const int TextType = 1, FileType = 2;

        Field();
        virtual ~Field();

        void SetType(int type);
        int GetType();

        void AcceptSomeData(char *data, long length);


        // File functions
        void SetUploadedFilesStorage(int where);
        void SetTempDir(std::string dir);

        void SetFileName(std::string name);
        std::string GetFileName();

        void SetFileContentType(std::string type);
        std::string GetFileMimeType();

        char * GetFileContent();
        unsigned long GetFileContentSize();

        std::string GetTempFileName();

        // Text field operations
        std::string GetTextTypeContent();




    private:
        unsigned long FieldContentLength = 0;

        int WhereToStoreUploadedFiles = 0;

        std::string TempDir = "", TempFile = "";
        std::string FileContentType = "", FileName = "";

        int type = 0;
        char * FieldContent;
        std::ofstream file;

    };
}

#endif //SHIRO_FIELD_HH
