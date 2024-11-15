#include "../DelilaSelectorEliade.h"



void DelilaSelectorEliade::AnalyseQuOliver(){
    
            
           
    
            if (blCS)                   cs_simple(35);
            if (blGammaGamma)           TreatGammaGammaCoinc();
            if (my_confs["Fold"])                 TreatFold(3);
            if (my_confs["DeeSector"])		    ViewDeESector();
            if (blFillSingleSpectra)    FillSingleSpectra();
//             std::cout<<"Warning  DelilaEvent.TimeBunch_ is more than rf_time \n";
            
            hdelilaQu_size->Fill(delilaQu.size());
            
            delilaQu.clear();
//             MovePreQu2QuOliver();//this
//             delilaPreQu.clear();//this
            return;
}


void DelilaSelectorEliade::EventBuilderForOliver()
{
//     if (blIsWindow){//close the event when the real trigger signal arrives

    if (DelilaEvent_.det_def == external_trigger_det_def){ //as the next real trigger comes, close event
        
        
        hTriggerTrigger->Fill(DelilaEvent_.Time - LastTriggerEvent.Time);
        hTriggerDomain->Fill(DelilaEvent_.domain);

        LastTriggerEvent = DelilaEvent_;
        LastTriggerEvent = DelilaEvent_;
        
        AnalyseQuOliver();
        
//         delilaPreQu.push_back(DelilaEvent_); // add the current event to be the first in the new queue //this
        
//         blIsWindow = false;  
        return; 
        
       }else if (DelilaEvent_.TimeBunch >=rf_time){
                   
                AnalyseQuOliver();
           
//     } else if (DelilaEvent_.TimeBunch <= my_params["window_length"] ){
    } else if (DelilaEvent_.TimeBunch <= rf_time  ){    
                hTimeInBunch->Fill(DelilaEvent_.TimeBunch);         
                delilaQu.push_back(DelilaEvent_);
//     } else if (DelilaEvent_.TimeBunch  > rf_time - my_params["pre_window"]){
//         DelilaEvent_.TimeBunch = DelilaEvent_.TimeBunch -  my_params["pre_window"];
//         delilaPreQu.push_back(DelilaEvent_);   //this
        
//         std::cout<<" DelilaEvent_.TimeBunch  < rf_time - my_params[pre_window]" <<DelilaEvent_.TimeBunch <<" "<< rf_time - my_params["pre_window"] <<"\n";
//     }else if (DelilaEvent_.TimeBunch > my_params["window_length"]) {//this
//                 delilaPreQu.push_back(DelilaEvent_);   //this
    };//this
    
    return;
};
       
void DelilaSelectorEliade::MovePreQu2QuOliver()
{
    if (delilaPreQu.empty()) return;
    if (pre_event_length == 0) return;
    if (!delilaQu.empty()) {
        std::cout<<"CheckPreQu delilaQu is not empty \n";
        delilaQu.clear();
    };
    
    std::deque<DelilaEvent>::iterator it_= delilaPreQu.begin();
    double time_diff_temp;
    
    int nval = 0;
    
    for (; it_!= delilaPreQu.end();++it_){
           time_diff_temp = it_->TimeBunch - DelilaEvent_.TimeBunch;
           if (abs(time_diff_temp) < pre_event_length) {
               (*it_).TimeBunch = it_->TimeBunch - DelilaEvent_.TimeBunch ;
               delilaQu.push_back(*it_); nval++;
        };
    };
    
    hdelilaPreQu_size->Fill(nval);
    
     std::cout<<"PreQueSize is "<< delilaPreQu.size() << " \n";
     std::cout<<"delilaQueSize is "<< delilaQu.size() << " \n";
    
    delilaPreQu.clear();
}

void DelilaSelectorEliade::FillSpectraFromPreQu()
{
    std::deque<DelilaEvent>::iterator it_ev__= delilaPreQu.begin();
   
     for (; it_ev__!= delilaPreQu.end();++it_ev__){
         FillSpectraForOliver(*it_ev__);
     }
     delilaPreQu.clear();
   
};

void DelilaSelectorEliade::FillSpectraForOliver(DelilaEvent event)
{
    
    
//     if (event.TimeBunch > 3.5e5) {
//         event.TimeBunch-=4e5;
//         delilaPreQu.push_back(event);
//         return;
//     };
//         
        
        
    double TimeBunchOliver = event.TimeBunch;
//      if (event.TimeBunch > 3.5e5) TimeBunchOliver-= 4e5; 
    
    
    if (event.det_def == 5) {
        mEnergyTimeDiff[event.det_def]->Fill(event.Energy_kev, TimeBunchOliver);
        return;
    };
    
    if (event.det_def == 3){
        mEnergyTimeDiff[event.det_def]->Fill(event.Energy_kev, TimeBunchOliver);
        if ((event.CS == 0 && blCS)){
            mEnergyTimeDiffCS[event.det_def]->Fill(event.Energy_kev, TimeBunchOliver);
            mDelilaCS->Fill(event.domain, event.Energy_kev);
            hDelilaCS[event.det_def]->Fill(event.Energy_kev);
        }
        if (my_params["beta"] > 0) {
            mEnergyTimeDiffDC[event.det_def]->Fill(event.EnergyDC, TimeBunchOliver);
            mDelilaDC->Fill(event.domain, event.EnergyDC); 
            hDelilaDC[event.det_def]->Fill(event.EnergyDC);
            if ((event.CS == 0 ) && blCS){
                mEnergyTimeDiffCS_DC[event.det_def]->Fill(event.EnergyDC, TimeBunchOliver);
                mDelilaCS_DC->Fill(event.domain, event.EnergyDC);
                hDelilaCS_DC[event.det_def]->Fill(event.EnergyDC);
            };
        };
     };    
    return;
}

void DelilaSelectorEliade::FillSpectraForElifant(DelilaEvent event)
{
    if ((event.det_def == 1)|| (event.det_def == 3)) {
        if ((event.CS == 0) && blCS)  {
            mDelilaCS->Fill(event.domain, event.Energy_kev);
            hDelilaCS[event.det_def]->Fill(event.Energy_kev);
        };
        if (my_params["beta"] > 0) {
            mDelilaDC->Fill(event.domain, event.EnergyDC); 
            hDelilaDC[event.det_def]->Fill(event.EnergyDC);
            if ((event.CS == 0) && blCS)  {
                mDelilaCS_DC->Fill(event.domain, event.EnergyDC);
                hDelilaCS_DC[event.det_def]->Fill(event.EnergyDC);
            };
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


