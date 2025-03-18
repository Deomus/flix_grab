namespace ipc {

    enum Message
    {
        kMessage_Unknown = 0,
        kMessage_Invoke,        //Payload as Audio Configuration;
        kMessage_Result,             //int32_t as operation Result;
        kMessage_Error,             //int32_t as operation Result;
        kMessage_Finish,             //Cancel Process Messages;

    };

}