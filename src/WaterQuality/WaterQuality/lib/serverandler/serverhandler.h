#ifndef SERVERHANDLER_H
    #define SERVERHANDLER_H
    template <typename channelType, typename updateType>
    class serverHandler
    {
        channelType *comChannel;
        updateType *updater;
        String dataPostserver;
        String dataPostpath;
        String errorPostserver;
        String errorPostpath;
        String OTAserver;
        String OTAPostpath;
        serverHandler(channelType comChannel_, updateType updateClass, String dataPostserver_, String dataPostpath_, String errorPostserver_, String errorPostpath_, String OTAserver_, String OTAPostpath_){
            comChannel = &comChannel;
            dataPostserver = dataPostserver_;
            dataPostpath = dataPostpath_;
            errorPostserver = errorPostserver_;
            errorPostpath =  errorPostpath_;
            OTAserver = OTAserver_;
            OTAPostpath = OTAPostpath_;
            updater = updateClass;
        }

        int postData(String data){

        }

        int postError(String data){

        }

        int OTAcheck(){
            
        }


    }
#endif