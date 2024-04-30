#!/bin/bash

runnb=$1
runnb1=${2:-$runnb}
volume1=${3:-0}
volume2=${4:-$volume1}
nevents=${5:-0}
AddBAck=${6:-0}
server=${7:-2}


echo "Put Parameters: AddBack (0 - if none); server_nbr (0 - if none); run_nbr; volume_from; volume_to;"

echo "RUNfirst      $runnb"
echo "RUNlast       $runnb1"
echo "VOLUMEfirst   $volume1"
echo "VOLUMElast    $volume2"
echo "N EVENTS      $events"
echo "ADDBACK       $AddBAck"
echo "SERVER ID     $server"


lut_path="$HOME/onlineAnalysis/LookUpTables/2024_w15_oliver/"
lut_link="$HOME/EliadeSorting/"

#lut_file="LUT_ELIFANT_20240409_60Co.dat"
lut_file="LUT_ELIFANT_20240416_60Co.dat"
#lut_ta="TimeCalibMaxBin_R10038.dat"
#lut_ta="LUT_TA_TimeCalibMaxBin_R66.dat"
#lut_ta="LUT_TA_R71_TimeCalibGaussian.dat"
#lut_ta="LUT_TA_TimeCalibGaussian_R071_LABR.dat"
lut_ta="LUT_TA_TimeCalibGaussian_R071_Bunch.dat"
#lut_ta="LUT_TA_TimeCalibMaxBin_R167_Bunch.dat"
#lut_ta=""
#lut_conf="LUT_CONF_SI_TRG.dat"
lut_conf="LUT_CONF_LaBr_TRG.dat"

unlink "$lut_link""LUT_ELIADE.dat"
unlink "$lut_link""LUT_ELIADE.json"
unlink "$lut_link""LUT_TA.dat"
unlink "$lut_link""LUT_CONF.dat"

if [ "$runnb" -ge "166" ]; then
	lut_ta="LUT_TA_TimeCalibMaxBin_R167_Bunch.dat"
fi
      
###########################LUT_ELIADE###s#####################
if [ -z "$lut_file" ]
then
      echo "LUT_ELIADE.dat is missing"
else
      unlink "$lut_link""LUT_ELIADE.dat"
      ln -s "$lut_path$lut_file" "$lut_link""LUT_ELIADE.dat"
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
#############################################################

echo "--------------------------------------------------------"
echo "Setting of LUT(s)"
echo "--------------------------------------------------------"	
echo -ne "LUT_ELIADE file	: "; tput setaf 2; echo " $lut_file";tput sgr0;
echo -ne "LUT_CONF file	: "; tput setaf 2; echo " $lut_conf";tput sgr0;
echo -ne "LUT_TA file	: "; tput setaf 2; echo " $lut_ta";tput sgr0;
echo "--------------------------------------------------------"


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
