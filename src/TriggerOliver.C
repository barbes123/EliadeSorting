#include "../DelilaSelectorEliade.h"



void DelilaSelectorEliade::EventBuilderForOliver()
{
    if (blIsWindow){//open

        if (DelilaEvent_.det_def == external_trigger_det_def){
            if (blCS)                   cs_simple(35);
            if (blFold)                 TreatFold(3);
            if (blGammaGamma)           TreatGammaGammaCoinc();
            if (blDeeSector)		    ViewDeESector();
            if (blDeeRing)		        ViewDeERings();
           
           if (blFillSingleSpectra)     FillSingleSpectra();
//            if (blOutTree)               FillOutputTree();
           
           hdelilaQu_size->Fill(delilaQu.size());          
           delilaQu.clear();
           delilaPreQu.clear();
           
           blIsWindow = false;  
           return; 
           
        }else{
            
            if (DelilaEvent_.TimeBunch >=rf_time){
                if (blCS)                   cs_simple(35);
                if (blGammaGamma)           TreatGammaGammaCoinc();
                if (blFold)                 TreatFold(3);
                if (blDeeSector)		    ViewDeESector();
                if (blFillSingleSpectra)    FillSingleSpectra();
                std::cout<<"Warning  DelilaEvent.TimeBunch_ is more than rf_time \n";
                hdelilaQu_size->Fill(delilaQu.size());
                delilaQu.clear();
                } else if (DelilaEvent_.TimeBunch < event_length){
                    hTimeInBunch->Fill(DelilaEvent_.TimeBunch);         
                    delilaQu.push_back(DelilaEvent_);
              };
       };
       
   }else if (DelilaEvent_.det_def == external_trigger_det_def){//window is closed
        hTriggerTrigger->Fill(DelilaEvent_.Time - LastTriggerEvent.Time);
        hTriggerDomain->Fill(DelilaEvent_.domain);

        LastTriggerEvent = DelilaEvent_;
//         LastBunchEvent = DelilaEvent_;
        LastTriggerEvent = DelilaEvent_;
        blIsWindow = true;
   };
   return;
}


void DelilaSelectorEliade::FillSpectraForOliver(DelilaEvent event)
{
    if (event.det_def == 5) {
        mEnergyTimeDiff[event.det_def]->Fill(DelilaEvent_.Energy_kev, DelilaEvent_.TimeBunch);
        return;
    };
    
    if (event.det_def == 3){
        mEnergyTimeDiff[event.det_def]->Fill(DelilaEvent_.Energy_kev, DelilaEvent_.TimeBunch);
        if (event.CS == 0 )mEnergyTimeDiffCS[event.det_def]->Fill(DelilaEvent_.Energy_kev, DelilaEvent_.TimeBunch);
        if (beta > 0) {
            mEnergyTimeDiffDC[event.det_def]->Fill(DelilaEvent_.EnergyDC, DelilaEvent_.TimeBunch);
            if (event.CS == 0 )mEnergyTimeDiffCS_DC[event.det_def]->Fill(DelilaEvent_.EnergyDC, DelilaEvent_.TimeBunch);
        };
     };    
    return;
        
    
}

// void DelilaSelectorEliade::cs_in_bunch(int coinc_id)//for Oliver
// {
//     if (delilaQu.empty()) return;
//     
//    
//     double time_diff_bgo = -1;
//     std::deque<DelilaEvent>  ::iterator it_ev1__ = delilaQu.begin();
//     
//     int current_coinc_id = 0;
//     double time_diff= 0;
//     
//    
//      for (; it_ev1__ != delilaQu.end(); ++it_ev1__){
//          if ((*it_ev1__).TimeBunch < 0) continue;
//          if ((*it_ev1__).det_def > 6)   continue; //not bgo (4,5,6) or gamma (1,2,3)
// //          if ((*it_ev1__).CS == 1 )      continue; 
//          std::deque<DelilaEvent>  ::iterator it_ev2__ = delilaQu.begin();
//          
//  //                     std::cout<<"current_coinc_id x "<<current_coinc_id<<"\n";
//  
//         for (; it_ev2__ != delilaQu.end(); ++it_ev2__){
//              
//              if (it_ev1__ == it_ev2__)                           continue;
//              if ((*it_ev2__).TimeBunch < 0)                      continue;
//              if ((*it_ev2__).det_def > 6)                        continue;
// //              if ((*it_ev2__).CS == 1)                            continue;              
//              if ((*it_ev1__).det_def == (*it_ev2__).det_def)     continue;
//              if ((*it_ev1__).cs_domain != (*it_ev2__).cs_domain) continue;
// 
//              
//              if ((*it_ev1__).det_def < (*it_ev2__).det_def){current_coinc_id = (*it_ev1__).det_def*10+(*it_ev2__).det_def ;}
//                  else current_coinc_id = (*it_ev2__).det_def*10+(*it_ev1__).det_def;
// // //             std::cout<<"current_coinc_id "<<current_coinc_id<<"\n";
//  
//              if (current_coinc_id != coinc_id) continue;            
//              
//              
//              time_diff =  ((*it_ev1__).TimeBunch - (*it_ev2__).TimeBunch);
//              mTimeDiffCS ->Fill((*it_ev1__).cs_domain, time_diff);
//              
//              if (abs(time_diff) < coinc_gates[current_coinc_id]) 
// //                  if ((abs(time_diff) < 250e3) && (abs(time_diff) > 150e3) )
//                  {
//                      if ((*it_ev1__).det_def <= 3) {(*it_ev1__).CS = 1;}
//                         else if ((*it_ev2__).det_def <= 3) (*it_ev2__).CS = 1;
//                  };
//          };
//     };
// };


UShort_t DelilaSelectorEliade::AddToMask(DelilaEvent ev)
{
      
    vMask[0] = 0;
    vMask[1] = 0;
    vMask[2] = 0;
    
    UShort_t id = 10;
     
    switch (ev.det_def){
          case 3:
          {
                id = 0;
                break;
          }
          case 7:
          {
                id = 1;  
                break;
          }
          case 17:
          {
              id = 2;  
              break;
          }
          default:
          {
              break;
            }
      };
      
      if (id <3){
          vMaskEvents[id] = ev;
          vMask[id] = 1;
      }
     return id;
}

