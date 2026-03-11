#ifndef RTSPPARSER_H
#define RTSPPARSER_H
#include "GB28181Defs.h"
#include "GBMANSRTSP.h"
#include <stdlib.h>

class RtspParser
{
public:

    static void CmdToString(const PlayCtrlCmd* cmd, std::string& result);
    static bool StringToCmd(const std::string& input, std::string& cseq,  PlayCtrlCmd* cmd  );
    static std::string GetResponseMessage( int status,const std::string& description, const std::string& cseq  );
};

std::string GetNptTime(std::string& time)
{
      size_t head = time.find("npt=");
      if( head == std::string::npos){
           return "";
      }
      size_t tail = time.find("-");
      if( tail == std::string::npos){
           return "";
      }
      return time.substr( head+4, tail - head - 4  );
}


bool RtspParser::StringToCmd(const std::string& input, std::string& cseq,  PlayCtrlCmd* cmd  )
{
        CGBMANSRTSPReq   req;
        if(!req.parseFromStr((char*)input.c_str(),  input.length())){
            return false;
        }

       cseq = req.GetCseq();
       if( req.Method() == "PLAY"  ) {

           std::string scale = req.GetScaleStr();

           if(!scale.empty())  {

               float s = (float)atof(scale.c_str());
               if(   s > 1) {
                    cmd->Type =   kPlayFast;
               }else{
                     cmd->Type  = kPlaySlow;
               }
                cmd->Scale = s;

                return true;
           }

           std::string npt = req.GetRangeStr();
           std::string npt_time = GetNptTime(npt);

           if( npt_time.empty()){
               return false;
           }

           if( npt_time == "now" ) {
               cmd->Type  = kPlayStart;
           }
           else{
               cmd->Type  = kPlayDarg;
               cmd->Npt =  atoi(  npt_time.c_str() )  ;
           }
             return true;
       }

       if( req.Method() == "PAUSE") {
           cmd->Type = kPlayPause;
           return true;
       }

       if( req.Method() == "TEARDOWN"){
           cmd->Type = kPlayStart;
           return true;
       }
           return false;
}

std::string RtspParser::GetResponseMessage(int status,const std::string& description, const std::string& cseq)
{
       CGBMANSRTSPReq   req;
       return req.GetResponseMessage( status, description,  cseq );
}


void RtspParser::CmdToString(const PlayCtrlCmd* cmd, std::string& result)
{

      CGBMANSRTSPReq   req;

      switch(cmd->Type ) {
           case kPlayPause:
                    {
                       req.Method("PAUSE");
                       req.PushHeader("PauseTime","now");
                        result = req.ToStr();
                    }   return;
           default:
                      req.Method("PLAY");
      }

      char buffer[64] ={0};
      if( cmd->Scale > 0 ) {
             sprintf(buffer,  "%f",cmd->Scale);
             req.PushHeader("Scale", buffer);
	  }
	  else {
		  if (cmd->Npt > 0) {

			  sprintf(buffer, "%d", cmd->Npt);
			  std::string range = "npt=";
			  range.append(buffer);
			  range.append("-");
			  req.PushHeader("Range", range.c_str());

		  }
		  else {
			  req.PushHeader("Range", "npt=now-");
		  }
	  }
       result = req.ToStr();
       return;
}


#endif // RTSPPARSER_H
