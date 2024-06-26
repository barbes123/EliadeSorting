#ifndef DELILA_EVENT_H_INCLUDED
#define DELILA_EVENT_H_INCLUDED


#include <map>
#include <vector>
#include <climits>


 class DelilaEvent {
 public:
//     UChar_t	        fMod; 
//     UChar_t	        fChannel; 
    UShort_t	    fEnergy;//ChargeLong
    UShort_t	    fEnergyShort;//ChargeShot  
    UShort_t        det_def;//0 - nothing; 1 - core/single HPge; 2 - segment; 3 - CeBr; 4 - CsI; 5 - BGO1; 6 - BGO2; 7 -BGO - 3; 8 - solar cell; 9 - pulser
    float	        Energy_kev;
    float           EnergyDC;
//     UShort_t        domain;
    int             domain;
    int             core_id;
    int             particleID;
    UShort_t        cs_domain;
    int             channel;//ch daq
    UShort_t        CS;//0 - no; 1 - yes
    double          Time;
    double          TimeBunch;
    double          TimeTrg;
    Float_t         theta;
    Float_t	        phi;
    ULong64_t       trg;
    float           Amax; //add by saka//only for ELIASSA
    int             ring;
//     UShort_t        bunch;
//     double          TimeBunch;
    UShort_t        fold;    
    UShort_t        coincID;
    UShort_t        coreID;//coreID for ELIADE 
    UShort_t        segmentID;//only for ELIADE
    UShort_t        cloverID;//only for ELIADE
    float           ElasticEnergy; 
    float	        e_energy;

    DelilaEvent(): domain(-1),channel(-1),fEnergy(-1),CS(0),cs_domain(0),Time(0),trg(0),Amax(0),coincID(0),coreID(0),segmentID(0),TimeBunch(0),TimeTrg(0),e_energy(0),particleID(0){};
 };


#endif
