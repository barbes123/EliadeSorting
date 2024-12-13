#include "../DelilaSelectorEliade.h"



void DelilaSelectorEliade::AnalyseQuOliver(){
    
            
           
    
            if (blCS)                   cs_simple(35);
            if (blGammaGamma)           TreatGammaGammaCoinc();
            if (my_confs["Fold"])                 TreatFold(3);
            if (my_confs["DeeSector"])		    ViewDeESector();
            
            TreatGammaEdetectorCoincOliver(37);
            
            if (my_confs["TOF"]) TreatNeutronPartCoinc(88);
            
            
            //---------------------------------------------
            if (blFillSingleSpectra)    FillSpectraForOliver();
            //---------------------------------------------
            
            
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
//     std::deque<DelilaEvent>::iterator it_ev__= delilaPreQu.begin();
//    
//      for (; it_ev__!= delilaPreQu.end();++it_ev__){
//          FillSpectraForOliver(*it_ev__);
//      }
//      delilaPreQu.clear();
    return;
   
};

void DelilaSelectorEliade::FillSpectraForOliver()
{
     std::deque<DelilaEvent>::iterator it_ev__= delilaQu.begin();
   
     for (; it_ev__!= delilaQu.end();++it_ev__){
         
         int domain = (*it_ev__).domain;
         
        hDelila_single[(*it_ev__).det_def]->Fill((*it_ev__).Energy_kev);
        

        if ((*it_ev__).det_def == 3 || (*it_ev__).det_def == 1) {
            if (((*it_ev__).CS == 0) && blCS)  {
                mDelilaCS->Fill(domain, DelilaEvent_.Energy_kev);
                hDelilaCS[(*it_ev__).det_def]->Fill((*it_ev__).Energy_kev);
            };
            if (my_params["beta"] > 0) {
                mDelilaDC->Fill(domain, DelilaEvent_.EnergyDC); 
                hDelilaDC[(*it_ev__).det_def]->Fill((*it_ev__).EnergyDC);
                if (((*it_ev__).CS == 0) && blCS)  {
                    mDelilaCS_DC->Fill(domain, DelilaEvent_.EnergyDC);
                    hDelilaCS_DC[(*it_ev__).det_def]->Fill((*it_ev__).EnergyDC);
                 };
             };
         };
         
         
         if ((*it_ev__).det_def == 5) {
             mEnergyTimeDiff[(*it_ev__).det_def]->Fill((*it_ev__).Energy_kev, (*it_ev__).TimeBunch);
 //             return;
         };
    
        if ((*it_ev__).det_def == 3){
            
            hPID -> Fill((*it_ev__).particleID);
            
            mEnergyTimeDiff[(*it_ev__).det_def]->Fill((*it_ev__).Energy_kev, (*it_ev__).TimeBunch);
            if (((*it_ev__).CS == 0 && blCS)){
    //             mEnergyTimeDiffCS[(*it_ev__).det_def]->Fill((*it_ev__).Energy_kev, TimeBunchOliver);//too many big hists for memory
                mDelilaCS->Fill((*it_ev__).domain, (*it_ev__).Energy_kev);
                hDelilaCS[(*it_ev__).det_def]->Fill((*it_ev__).Energy_kev);
            };
        
            if (my_params["beta"] > 0) {
                
//                 std::cout<<" PID fill "<< (*it_ev__).particleID<< "\n";

    //             mEnergyTimeDiffDC[(*it_ev__).det_def]->Fill((*it_ev__).EnergyDC, TimeBunchOliver);//too many big hists for memory
                mDelilaDC->Fill((*it_ev__).domain, (*it_ev__).EnergyDC); 
                hDelilaDC[(*it_ev__).det_def]->Fill((*it_ev__).EnergyDC);
                
                if (((*it_ev__).CS == 0 ) && blCS){
                    mEnergyTimeDiffCS_DC[(*it_ev__).det_def]->Fill((*it_ev__).EnergyDC,  (*it_ev__).TimeBunch);
                    mDelilaCS_DC->Fill((*it_ev__).domain, (*it_ev__).EnergyDC);
                    hDelilaCS_DC[(*it_ev__).det_def]->Fill((*it_ev__).EnergyDC);
                    
//                    
                    if ((*it_ev__).particleID > 0){
                        mEnergyTimeDiffCS_DC_E[(*it_ev__).det_def]->Fill((*it_ev__).EnergyDC, (*it_ev__).TimeBunch);
                    }
                    else{
                        mEnergyTimeDiffCS_DC_noE[(*it_ev__).det_def]->Fill((*it_ev__).EnergyDC, (*it_ev__).TimeBunch);
                    };                
                };
            };
        };    
     };
}


// void DelilaSelectorEliade::FillSpectraForOliver(DelilaEvent event)
// {
//     
//     
// //     if (event.TimeBunch > 3.5e5) {
// //         event.TimeBunch-=4e5;
// //         delilaPreQu.push_back(event);
// //         return;
// //     };
// //         
//         
//         
//     double TimeBunchOliver = event.TimeBunch;
// //      if (event.TimeBunch > 3.5e5) TimeBunchOliver-= 4e5; 
//     
//     
//     if (event.det_def == 5) {
//         mEnergyTimeDiff[event.det_def]->Fill(event.Energy_kev, TimeBunchOliver);
//         return;
//     };
//     
//     if (event.det_def == 3){
//         mEnergyTimeDiff[event.det_def]->Fill(event.Energy_kev, TimeBunchOliver);
//         if ((event.CS == 0 && blCS)){
// //             mEnergyTimeDiffCS[event.det_def]->Fill(event.Energy_kev, TimeBunchOliver);//too many big hists for memory
//             mDelilaCS->Fill(event.domain, event.Energy_kev);
//             hDelilaCS[event.det_def]->Fill(event.Energy_kev);
//         }
//         if (my_params["beta"] > 0) {
//             
//             std::cout<<" PID "<< event.particleID<< "\n";
// 
// //             mEnergyTimeDiffDC[event.det_def]->Fill(event.EnergyDC, TimeBunchOliver);//too many big hists for memory
//             mDelilaDC->Fill(event.domain, event.EnergyDC); 
//             hDelilaDC[event.det_def]->Fill(event.EnergyDC);
//             if ((event.CS == 0 ) && blCS){
//                 mEnergyTimeDiffCS_DC[event.det_def]->Fill(event.EnergyDC, TimeBunchOliver);
//                 mDelilaCS_DC->Fill(event.domain, event.EnergyDC);
//                 hDelilaCS_DC[event.det_def]->Fill(event.EnergyDC);
//                 
//                 hPID -> Fill(event.particleID);
//                 if (event.particleID > 0){
//                     mEnergyTimeDiffCS_DC_E[event.det_def]->Fill(event.EnergyDC, TimeBunchOliver);
//                 }
//                 else{
//                     mEnergyTimeDiffCS_DC_noE[event.det_def]->Fill(event.EnergyDC, TimeBunchOliver);
//                 };                
//             };
//         };
//      };    
//     return;
// }

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


// void DelilaSelectorEliade::TreatGammaEdetectorCoinc(int id_gamma_det=3, int id_e_det=7)
void DelilaSelectorEliade::TreatGammaEdetectorCoincOliver(int coinc_id)
{
    
//    if (coinc_id != 1771 && coinc_id != 1773) {
//        std::cout<<"TreatGammaPartCoinc wrong coinc_id \n";
//        return;
//    }
   std::deque<DelilaEvent>::iterator it_e_= delilaQu.begin();
   std::deque<DelilaEvent>::iterator it_g_= delilaQu.begin();
   
   int id_gamma_det=coinc_id/10;
   int id_e_det=coinc_id%10;
   
//     std::cout<<"id_gamma_det "<<id_gamma_det<<" \n";
//     std::cout<<"id_e_det "<<id_e_det<<" \n";
   
    for (; it_e_  != delilaQu.end();++it_e_){
        if ((*it_e_).det_def != id_e_det) continue;
        
//         std::cout<<"(*it1_).det_def "<<(*it1_).det_def<<" (*it1_).e_energy "<<(*it1_).e_energy<<" \n";

        
//         if ((*it_e_).e_energy == 0) continue;
        if ((*it_e_).fEnergy == 0) continue;
        
        it_g_= delilaQu.begin();
        
        for (; it_g_  != delilaQu.end();++it_g_){
          
//               std::cout<<"cccc"<<"\n";
//             if ((*it2_).det_def != dname["LaBr"]) continue;
            if ((*it_g_).det_def != id_gamma_det) continue;
            if ((*it_g_).CS != 0) continue;
            
            double time_diff ;
            
            if (blExtTrigger) 
            {
                time_diff = (*it_e_).TimeBunch - (*it_g_).TimeBunch;
            }else time_diff = (*it_e_).Time - (*it_g_).Time; 
            
            
//             hTimeDiff_g_e->Fill(time_diff);
            
//             std::cout<<time_diff<<"\n";
            if (abs(time_diff) < coinc_gates[coinc_id]) it_g_->particleID+=1;
            
//             if ( it_g_->particleID > 0) std::cout<<" <PID> " <<it_g_->particleID<<"\n";
        };
    };
    
    return;  

};

void DelilaSelectorEliade::TreatNeutronPartCoinc(int neutron_det_id)
{
	std::deque<DelilaEvent>::iterator it_= delilaQu.begin();
	 for (; it_  != delilaQu.end();++it_){
		 if ((*it_).det_def != neutron_det_id )continue;
	 //check if inside PSD cut
	 
		 std::map<UInt_t, string>::iterator it_pid_=particle_name_in_cut.begin();
		 for(;it_pid_!=particle_name_in_cut.end();++it_pid_){
		        if (particle_cut[it_pid_->second] == 0x0) continue;
		        
		        float psd = (1.0*(*it_).fEnergy - (*it_).fEnergyShort) / ((*it_).fEnergy + (*it_).fEnergyShort);
//			mPSD[domain]->Fill(DelilaEvent_.fEnergy, psd);		        
		        
		        if (particle_cut[it_pid_->second]->IsInside(it_->fEnergy,psd))//Energy_kev is E energy
		        {
		            (*it_).particleID = 10000;
		            //hPID_dee->Fill(it_->particleID);
		            //mdee_gate_check[it_->particleID]->Fill(it_->fEnergy, psd);
		            
		            mTOF_PSD_ch_long[it_->domain]->Fill(it_->fEnergy, it_->TimeBunch);
			    mTOF_PSD_ch_short[it_->domain]->Fill(psd, it_->TimeBunch);
			    mTOF_MEV[it_->domain]->Fill(Time2Energy(my_params["distance"],(it_->TimeBunch*pow(10,-12))));
		            
		            break;
		//                 it1_->particleID+= it_pid_->first;
		//                 mdee_gate_check[it_pid_->first]->Fill(it1_->e_energy, it1_->Energy_kev);
		//                 hGG_particle[it_pid_->second]->Fill((*it_g_).Energy_kev);
		            }
		        };
 
	 }


	return;
};


