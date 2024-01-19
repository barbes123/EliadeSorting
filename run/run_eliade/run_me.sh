#!/bin/bash

runnb=$1
runnb1=${2:-$runnb}
volume1=${3:-0}
volume2=${4:-$volume1}
nevents=${5:-0}
AddBAck=${6:-0}
server=${7:-3}



TESTLUTs=0

echo "Put Parameters: AddBack (0 - if none); server_nbr (0 - if none); run_nbr; volume_from; volume_to;"

echo "RUNfirst      $runnb"
echo "RUNlast       $runnb1"
echo "VOLUMEfirst   $volume1"
echo "VOLUMElast    $volume2"
echo "N EVENTS      $events"
echo "ADDBACK       $AddBAck"
echo "SERVER ID     $server"


lut_path="$HOME/onlineEliade/LookUpTables/s3/"
lut_link="$HOME/EliadeSorting/"


lut_file="LUT_S3_CL32_run32_raluca.dat"
lut_ta=""
lut_conf="LUT_CONF_S3_CL32.dat"
lut_json="LUT_S3_CL32_run32_raluca.json"
lut_acs="LUT_ACS.dat"
lut_ab="AddBack_distances_real.dat"
#lut_json=""

unlink "$lut_link""LUT_ELIADE.dat"
unlink "$lut_link""LUT_ELIADE.json"
unlink "$lut_link""LUT_TA.dat"
unlink "$lut_link""LUT_CONF.dat"

if [ "$runnb" -eq "12346" ]; then
	lut_file="LUT_ELIADE_ACS_run12346_PUBE_DT_many_peaks.dat";
#	lut_file="LUT_ELIADE_ACS_run12343_PUBE_DT.dat";
	lut_ta="TimeCalib_run12317_dom159.dat"	
fi


if [ "$runnb" -eq "12343" ]; then
	lut_file="LUT_ELIADE_ACS_run12343_PUBE_DT.dat";
	lut_ta="TimeCalib_run12317_dom159.dat"	
fi

if [ "$runnb" -eq "12341" ]; then
	lut_file="LUT_ELIADE_ACS_run12343_PUBE_DT_many_peaks.dat";
#	lut_file="LUT_ELIADE_ACS_run12343_PUBE_DT.dat";
	lut_ta="TimeCalib_run12317_dom159.dat"	
fi

if [ "$runnb" -eq "12340" ]; then
	lut_file="LUT_ELIADE_ACS_run12343_PUBE_DT_many_peaks.dat";
#	lut_file="LUT_ELIADE_ACS_run12343_PUBE_DT.dat";
	lut_ta="TimeCalib_run12317_dom159.dat"	
fi

if [ "$runnb" -eq "12330" ]; then
	lut_file="LUT_ELIADE_ACS_run12330_DT.dat";
	lut_ta="TimeCalib_run12317_dom159.dat"	
fi
if [ "$runnb" -eq "12317" ]; then
	lut_file="LUT_ELIADE_ACS_run12317.dat";
	lut_ta="TimeCalib_run12317_dom159.dat"	
fi
if [ "$runnb" -eq "12228" ]; then
	lut_file="LUT_ELIADE_ACS_run12228.dat";
	lut_ta="TimeCalib_run_12228_20220614.dat"
fi


      
###########################LUT_ELIADE#########################
if [ -z "$lut_file" ]
then
      echo "LUT_ELIADE.dat is missing"
else
      unlink "$lut_link""LUT_ELIADE.dat"
      ln -s "$lut_path$lut_file" "$lut_link""LUT_ELIADE.dat"
fi

###########################LUT_ELIADE#########################
if [ -z "$lut_json" ]
then
      echo "LUT_ELIADE.json is missing"
else
      unlink "$lut_link""LUT_ELIADE.json"
      ln -s "$lut_path$lut_json" "$lut_link""LUT_ELIADE.json"
      unlink "$lut_link""LUT_ELIADE.dat"
      lut_file=""
      echo "LUT_ELIADE.json is found; LUT_ELIADE.dat will not be used"
fi

###########################LUT_TA############################
if [ -z "$lut_ta" ]
then
      echo "LUT_TA.dat is missing"
else
      unlink "$lut_link""LUT_TA.dat"
      ln -s "$lut_path$lut_ta" "$lut_link""LUT_TA.dat"
fi

###########################LUT_CONF##########################
if [ -z "$lut_conf" ]
then
      echo "LUT_CONF.dat is missing"
else
      unlink "$lut_link""LUT_CONF.dat"
      ln -s "$lut_path$lut_conf" "$lut_link""LUT_CONF.dat"
fi

###########################LUT_AB##########################
if [ -z "$lut_conf" ]
then
      echo "LUT_AB.dat is missing"
else
      unlink "$lut_link""LUT_AB.dat"
      ln -s "$lut_path$lut_ab" "$lut_link""LUT_CONF.dat"
fi


###########################LUT_ACS##########################
if [ -z "$lut_conf" ]
then
      echo "LUT_ACS.dat is missing"
else
      unlink "$lut_link""LUT_ACS.dat"
      ln -s "$lut_path$lut_acs" "$lut_link""LUT_ACS.dat"
fi
#############################################################

echo "--------------------------------------------------------"
echo "Setting of LUT(s)"
echo "--------------------------------------------------------"	
echo -ne "LUT FOLDER 	: "; tput setaf 2; echo " $lut_path";tput sgr0;
echo -ne "LUT_ELIADE dat 	: "; tput setaf 2; echo " $lut_file";tput sgr0;
echo -ne "LUT_ELIADE json	: "; tput setaf 2; echo " $lut_json";tput sgr0;
echo -ne "LUT_CONF file	: "; tput setaf 2; echo " $lut_conf";tput sgr0;
echo -ne "LUT_TA file	: "; tput setaf 2; echo " $lut_ta";tput sgr0;
echo -ne "LUT_AB file	: "; tput setaf 2; echo " $lut_ab";tput sgr0;
echo -ne "LUT_ACS file	: "; tput setaf 2; echo " $lut_acs";tput sgr0;
echo "--------------------------------------------------------"

if [ "$TESTLUTs" -eq "1" ]; then
	echo "Testing LUTs; to run the Selector change TESTLUTs to 0"
	exit	
fi


while test $runnb -le $runnb1
do
    volnb=$volume1
    while test $volnb -le $volume2
    do
    echo "Now I am starting run the selector run$runnb"_"$volnb.root"	
    rootcommand=start_me.C+"($AddBAck,$server,$runnb,$volnb,$nevents)"    
    root -l -b -q $rootcommand    
    echo "I finished run$runnb"_"$volnb.root"
    volnb=$(($volnb + 1))      
    done
    runnb=$(($runnb + 1))  
done 
