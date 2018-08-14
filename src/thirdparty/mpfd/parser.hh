#ifndef SHIRO_PARSER_HH
#define SHIRO_PARSER_HH

#include <iostream>
#include <string>
#include <map>
#include "exception.hh"
#include "field.hh"
#include <string.h>
#include <stdlib.h>

namespace MPFD {

    class Parser {
    public:
        static const int StoreUploadedFilesInFilesystem = 1, StoreUploadedFilesInMemory = 2;


        Parser();
        ~Parser();

        void SetContentType(const std::string type);

        void AcceptSomeData(const char *data, const long length);



        void SetMaxCollectedDataLength(long max);
        void SetTempDirForFileUpload(std::string dir);
        void SetUploadedFilesStorage(int where);

        std::map<std::string, Field *> GetFieldsMap();
        Field * GetField(std::string Name);

    private:
        int WhereToStoreUploadedFiles;

        std::map<std::string, Field *> Fields;

        std::string TempDirForFileUpload;
        int CurrentStatus;

        // Work statuses
        static int const Status_LookingForStartingBoundary = 1;
        static int const Status_ProcessingHeaders = 2;
        static int const Status_ProcessingContentOfTheField = 3;

        std::string Boundary;
        std::string ProcessingFieldName;
        bool _HeadersOfTheFieldAreProcessed;
        long ContentLength;
        char *DataCollector;
        long DataCollectorLength, MaxDataCollectorLength;
        bool FindStartingBoundaryAndTruncData();
        void _ProcessData();
        void _ParseHeaders(std::string headers);
        bool WaitForHeadersEndAndParseThem();
        void TruncateDataCollectorFromTheBeginning(long n);
        long BoundaryPositionInDataCollector();
        bool ProcessContentOfTheField();
    };
}

#endif //SHIRO_PARSER_HH
