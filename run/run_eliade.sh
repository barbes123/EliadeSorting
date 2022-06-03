#!/bin/bash

runnb=$1
#LASTrun=$3
volume1=${2:-0}
volume2=${3:-$volume1}
AddBAck=${4:-0}
server=${5:-5}
nevents=${6:-0}
#VAR=${1:-DEFAULTVALUE}    
echo "Put Parameters: AddBack (0 - if none); server_nbr (0 - if none); run_nbr; volume_from; volume_to;"

lut_path="$HOME/onlineEliade/LookUpTables/"
lut_link="$HOME/EliadeSorting/"


#lut_file="LUT_ELIADE_ACS.dat"
lut_file="LUT_ELIADE_S5_3clovers.dat"
#lut_file="LUT_ELIADE_S5_3pulsers.dat"
#lut_file="LUT_ELIADE_S5_cut.dat"

lut_conf="coinc_gates_test_new.dat"

echo "$lut_path$lut_file" "$lut_link""LUT_ELIADE.dat"

unlink "$lut_link""LUT_ELIADE.dat"
ln -s "$lut_path$lut_file" "$lut_link""LUT_ELIADE.dat"

echo "--------------------------------------------------------"
echo "Setting of LUT(s)"
echo "--------------------------------------------------------"	
echo -ne "LUT  file:: "; tput setaf 2; echo " $lut_file";tput sgr0;
echo -ne "CONF file:: "; tput setaf 2; echo " $lut_conf";tput sgr0;
echo "--------------------------------------------------------"

volnb=$volume1
while test $volnb -le $volume2
do 
  echo "Now I am starting run the selector run$runnb"_"$volnb.root"	
  rootcommand=sorting_eliade.C+"($AddBAck,$server,$runnb,$runnb,$volnb,$volnb,$nevents)"    
  root -l -b -q $rootcommand
  volnb=$(($volnb + 1))  
  echo "I finished run$runnb_$volnb.root"
done 


#  
#  
# case  $DIST in
# ##########################################################################################################
# "20")	if [ "$FIRST" -eq "0" ]; then 
# 	   FIRST=2000 LAST=2013
# 	fi
# 	
# 	echo "Will run the selector for run mass $MASS for runs from $FIRST up to $LAST"	
# 	
# 	runnb=$FIRST
# 	while test $runnb -le $LAST
# 	do
# 	
# 	eu_lut_file="LUT_EUCLIDES_w46_Testov_zero_time.dat";	
# 	eu_cut_file="euclides_cuts_w46_testov_300um_all.root";
# 	ga_lut_file="LUT_GALILEO_w46_Testov_run_2000.dat";
# 	
# 	
# 	if [ "$runnb" -eq "2000" ]; then 
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2000.dat";
# 	   eu_cut_file="euclides_cuts_run_2000_20um.root";
# 	fi
# 	
# 	if [ "$runnb" -ge "2001" ] && [ "$runnb" -le "2004" ]; then
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2001.dat";
# 	   eu_cut_file="euclides_cuts_run_2000_20um.root";
# 	fi
# 	
# 	if [ "$runnb" -eq "2005" ]; then
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2005.dat";
# 	   eu_cut_file="euclides_cuts_run_2013_20um.root";
# 	fi
# 	
# 	if [ "$runnb" -eq "2006" ]; then
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2006.dat";
# 	   eu_cut_file="euclides_cuts_run_2013_20um.root";
# 	fi
# 	
# 	if [ "$runnb" -eq "2007" ]; then
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2007.dat";
# 	   eu_cut_file="euclides_cuts_run_2013_20um.root";
# 	fi
# 	
# 	if [ "$runnb" -ge "2009" ]; then
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2013.dat";
# 	   eu_cut_file="euclides_cuts_run_2013_20um.root";
# 	fi
# 	
# 	echo "--------------------------------------------------------"
# 	echo "Distance is set to $DIST um file [$file_prefix$runnb.root"]
# 	echo "--------------------------------------------------------"	
# 	echo -ne "LUT file:: "; tput setaf 2; echo " $ga_lut_file";tput sgr0;
# 	echo -ne "LUT file:: "; tput setaf 2; echo " $eu_lut_file";tput sgr0;
# 	echo -ne "CUT file:: "; tput setaf 2; echo " $eu_cut_file";tput sgr0;
# 	echo -ne "DATA file  "; tput setaf 6; echo " $data_path$file_prefix$runnb.root";tput sgr0;
# 	echo "LUT dir::   $lut_path"
# 	echo "CUT dir::   $cut_path"
# 	if [ "$TEST" -eq "1" ]; then
# 	tput setaf 1; echo "Testing: 1000000 events...."; tput sgr0;    
# 	fi
# 	echo "--------------------------------------------------------"
# 
# 
# 	#echo "$cut_path""cut_charged_particles.root"
# 	#echo "$cut_path""$eu_cut_file" "$cut_path""cut_charged_particles.root"
# 	unlink "$cut_path""cut_charged_particles.root"
# 	ln -s "$cut_path$eu_cut_file" "$cut_path""cut_charged_particles.root"
# 
# 	unlink "$lut_path""LUT_EUCLIDES.dat"
# 	ln -s "$lut_path$eu_lut_file" "$lut_path""LUT_EUCLIDES.dat"
# 
# 	unlink "$lut_path""LUT_GALILEO.dat"
# 	ln -s "$lut_path$ga_lut_file" "$lut_path""LUT_GALILEO.dat"	
# 	
#         if test -f "$data_path$file_prefix$runnb.root"
# 	    then	
# 		echo $rootcommand
# 		echo starting "$inputpath/$prefix$runnb.root"        
# 		rootcommand=automated_selector.C"($runnb,$runnb,$DIST,$TEST)" 
# 		echo "rootcommand $rootcommand"
# 		if [ $FLAG -eq 1 ]; then	    
# 		    root -l -b -q $rootcommand
# 		    else 
# 		    echo "<<<bash-script test>>"
# 		fi
# 	    fi
# 	    runnb=$(($runnb + 1))
# 	done
# 	FINISHED=1;
# 	;;
# ##########################################################################################################	
# "100")	if [ "$FIRST" -eq "0" ]; then #100 um from November 2016; december 2016 is below
# 	   FIRST=2100 LAST=2102
# 	fi
# 	
# 	echo "Will run the selector for run mass $MASS for runs from $FIRST up to $LAST"	
# 	
# 	runnb=$FIRST
# 	while test $runnb -le $LAST
# 	do
# 	
# 	eu_lut_file="LUT_EUCLIDES_w46_Testov_100um.dat";
# 	eu_cut_file="euclides_cuts_w46_testov_300um_all.root";
# 	ga_lut_file="LUT_GALILEO_w46_Testov_100um.dat";	
# 	
# 	if [ "$runnb" -eq "2101" ]; then 
# 	   ga_lut_file="LUT_GALILEO_w46_Testov_run_2101.dat";
# 	fi	
# 	
# 	echo "--------------------------------------------------------"
# 	echo "Distance is set to $DIST um file [$file_prefix$runnb.root"]
# 	echo "--------------------------------------------------------"	
# 	echo -ne "LUT file:: "; tput setaf 2; echo " $ga_lut_file";tput sgr0;
# 	echo -ne "LUT file:: "; tput setaf 2; echo " $eu_lut_file";tput sgr0;
# 	echo -ne "CUT file:: "; tput setaf 2; echo " $eu_cut_file";tput sgr0;
# 	echo -ne "DATA file  "; tput setaf 6; echo " $data_path$file_prefix$runnb.root";tput sgr0;
# 	echo "LUT dir::   $lut_path"
# 	echo "CUT dir::   $cut_path"
# 	if [ "$TEST" -eq "1" ]; then 
# 
# 	tput setaf 1; echo "Testing: 1000000 events...."; tput sgr0;    
# 	fi
# 	echo "--------------------------------------------------------"
# 
# 
# 	#echo "$cut_path""cut_charged_particles.root"
# 	#echo "$cut_path""$eu_cut_file" "$cut_path""cut_charged_particles.root"
# 	unlink "$cut_path""cut_charged_particles.root"
# 	ln -s "$cut_path$eu_cut_file" "$cut_path""cut_charged_particles.root"
# 
# 	unlink "$lut_path""LUT_EUCLIDES.dat"
# 	ln -s "$lut_path$eu_lut_file" "$lut_path""LUT_EUCLIDES.dat"
# 
# 	unlink "$lut_path""LUT_GALILEO.dat"
# 	ln -s "$lut_path$ga_lut_file" "$lut_path""LUT_GALILEO.dat"	
# 	
#         if test -f "$data_path$file_prefix$runnb.root"
# 	    then	
# 		echo $rootcommand
# 		echo starting "$inputpath/$prefix$runnb.root"        
# 		rootcommand=automated_selector.C"($runnb,$runnb,$DIST,$TEST)" 
# 		echo "rootcommand $rootcommand"
# 		if [ $FLAG -eq 1 ]; then	    
# 		    root -l -b -q $rootcommand
# 		    else 
# 		    echo "<<<bash-script test>>"
# 		fi
# 	    fi
# 	    runnb=$(($runnb + 1))
# 	done
# 	FINISHED=1;
# 	;;
# ##########################################################################################################	
# "200")	if [ "$FIRST" -eq "0" ]; then 
# 	   FIRST=2200 LAST=2214 #run 2203 2205 2215 is activation 
# 	fi
# 	eu_lut_file="LUT_EUCLIDES_w46_Testov.dat";
# 	eu_cut_file="euclides_cuts_w46_testov_300um_all.root";
# 	ga_lut_file="LUT_GALILEO_w46_Testov.dat";
# 	
# 	
# 	if [ "$runnb" -ge "2200" ]; then % -ge
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2200.dat";
# 	   eu_cut_file="euclides_cuts_run_2013_20um.root";
# 	fi
# 	
# 	if [ "$runnb" -ge "2201" ]; then % -ge
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2201.dat";
# 	   eu_cut_file="euclides_cuts_run_2013_20um.root";
# 	fi
# 	
# 	if [ "$runnb" -ge "2202" ]; then % -ge
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2202.dat";
# 	   eu_cut_file="euclides_cuts_run_2013_20um.root";
# 	fi
# 	
# 	if [ "$runnb" -ge "2209" ]; then % -ge
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2210.dat";
# 	   eu_cut_file="euclides_cuts_run_2209_200um.root";
# 	fi
# 	
# 	if [ "$runnb" -ge "2210" ]; then % -ge
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2210.dat";
# 	   eu_cut_file="euclides_cuts_run_2210_200um.root";
# 	fi
# 	
# 	if [ "$runnb" -ge "2213" ]; then % -ge
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2213.dat";	  
# 	   eu_cut_file="euclides_cuts_run_2210_200um.root";#to be checked
# 	fi
# 	
# 	if [ "$runnb" -ge "2214" ]; then % -ge
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2214.dat";
# 	   eu_cut_file="euclides_cuts_run_2210_200um.root";#to be checked
# 	fi
# 	
# 	
# 	
# 	FINISHED=0;
# 	;;
# ##########################################################################################################	
# "300")	if [ "$FIRST" -eq "0" ]; then 
# 	   FIRST=2300 LAST=2314 #run 2302 2307 2308 2310 2311 are bad
# 	fi
# 	
# 	echo "Will run the selector for run mass $MASS for runs from $FIRST up to $LAST"	
# 	
# 	runnb=$FIRST
# 	while test $runnb -le $LAST
# 	do
# 	
# 	eu_lut_file="LUT_EUCLIDES_w46_Testov.dat";
# 	eu_cut_file="euclides_cuts_w46_testov_300um_all.root";
# 	ga_lut_file="LUT_GALILEO_w46_Testov.dat";
# 	
# 	if [ "$runnb" -eq "2300" ]; then 
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2300.dat";
# 	   eu_cut_file="eu_cuts_w46_testov_300um_run_2300.root";
#            #eu_cut_file="eu_cuts_w46_testov_300um_run_2300_1a_2a_1p.root";
# 	fi
# 	
# 	if [ "$runnb" -eq "2301" ]; then 
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2300.dat";
# 	   eu_cut_file="eu_cuts_w46_testov_300um_run_2300.root";
#            #eu_cut_file="eu_cuts_w46_testov_300um_run_2300_1a_2a_1p.root";
# 	fi
# 	
# 	if [ "$runnb" -eq "2303" ]; then 
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2300.dat";
# 	   eu_cut_file="eu_cuts_w46_testov_300um_run_2300.root";
#            #eu_cut_file="eu_cuts_w46_testov_300um_run_2300_1a_2a_1p.root";
# 	fi
# 	
# 	if [ "$runnb" -eq "2304" ]; then 
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2304.dat";
# 	   eu_cut_file="eu_cuts_w46_testov_300um_run_2300.root";
#            #eu_cut_file="eu_cuts_w46_testov_300um_run_2300_1a_2a_1p.root";
# 	fi
# 	
# 	if [ "$runnb" -eq "2305" ]; then 
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2305.dat";
#     	   eu_cut_file="eu_cuts_w46_testov_300um_run_2300.root";
#            #eu_cut_file="eu_cuts_w46_testov_300um_run_2300_1a_2a_1p.root";
# 	fi
# 	
# 	if [ "$runnb" -eq "2306" ]; then 
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2306.dat";
# 	   #eu_cut_file="eu_cuts_w46_testov_300um_run_2300_1a_2a_1p.root";
#   	   eu_cut_file="eu_cuts_w46_testov_300um_run_2300.root";
# #  	   eu_cut_file="eu_cuts_w46_testov_300um_run_2300_alpha_proton.root";
# 	fi
# 	
# 	if [ "$runnb" -eq "2309" ]; then 
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2306.dat";
# 	   eu_cut_file="eu_cuts_w46_testov_300um_run_2300.root";
#            #eu_cut_file="eu_cuts_w46_testov_300um_run_2300_1a_2a_1p.root";
# 	fi
# 	
# 	if [ "$runnb" -eq "2312" ]; then 
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2312.dat";
#  	   eu_cut_file="eu_cuts_w46_testov_300um_run_2300.root";
#            #eu_cut_file="eu_cuts_w46_testov_300um_run_2300_1a_2a_1p.root";
# 	fi
# 	
# 	if [ "$runnb" -eq "2313" ]; then 
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2313.dat";
# 	   #eu_cut_file="eu_cuts_w46_testov_300um_run_2313_1a_2a_1p.root";
#  	   eu_cut_file="eu_cuts_w46_testov_300um_run_2313.root";
# 	fi
# 	
# 	
# 	if [ "$runnb" -eq "2314" ]; then 
# 	   eu_lut_file="LUT_EUCLIDES_w46_Testov_run_2314.dat";
#  	   eu_cut_file="eu_cuts_w46_testov_300um_run_2314.root";
# 	   #eu_cut_file="eu_cuts_w46_testov_300um_run_2314_alpha_proton.root";
# 	   #eu_cut_file="eu_cuts_w46_testov_300um_run_2314_1a_2a_1p.root";
# 	fi
# 	
# 	echo "--------------------------------------------------------"
# 	echo "Distance is set to $DIST um file [$file_prefix$runnb.root"]
# 	echo "--------------------------------------------------------"	
# 	echo -ne "LUT file:: "; tput setaf 2; echo " $ga_lut_file";tput sgr0;
# 	echo -ne "LUT file:: "; tput setaf 2; echo " $eu_lut_file";tput sgr0;
# 	echo -ne "CUT file:: "; tput setaf 2; echo " $eu_cut_file";tput sgr0;
# 	echo -ne "DATA file  "; tput setaf 6; echo " $data_path$file_prefix$runnb.root";tput sgr0;
# 	echo "LUT dir::   $lut_path"
# 	echo "CUT dir::   $cut_path"
# 	if [ "$TEST" -eq "1" ]; then
# 	tput setaf 1; echo "Testing: 1000000 events...."; tput sgr0;    
# 	fi
# 	echo "--------------------------------------------------------"
# 
# 
# 	#echo "$cut_path""cut_charged_particles.root"
# 	#echo "$cut_path""$eu_cut_file" "$cut_path""cut_charged_particles.root"
# 	unlink "$cut_path""cut_charged_particles.root"
# 	ln -s "$cut_path$eu_cut_file" "$cut_path""cut_charged_particles.root"
# 
# 	unlink "$lut_path""LUT_EUCLIDES.dat"
# 	ln -s "$lut_path$eu_lut_file" "$lut_path""LUT_EUCLIDES.dat"
# 
# 	unlink "$lut_path""LUT_GALILEO.dat"
# 	ln -s "$lut_path$ga_lut_file" "$lut_path""LUT_GALILEO.dat"	
# 	
#         if test -f "$data_path$file_prefix$runnb.root"
# 	    then	
# 		echo $rootcommand
# 		echo starting "$inputpath/$prefix$runnb.root"        
# 		rootcommand=automated_selector.C"($runnb,$runnb,$DIST,$TEST)" 
# 		echo "rootcommand $rootcommand"
# 		if [ $FLAG -eq 1 ]; then	    
# 		    root -l -b -q $rootcommand
# 		    else 
# 		    echo "<<<bash-script test>>"
# 		fi
# 	    fi
# 	    runnb=$(($runnb + 1))
# 	done
# 	FINISHED=1;
# 	;;
# ##########################################################################################################	
# "1000")	if [ "$FIRST" -eq "0" ]; then 
# 	   FIRST=2400 LAST=2406 
# 	fi
# 	eu_lut_file="LUT_EUCLIDES_w46_Testov.dat";
# # 	eu_cut_file="euclides_cuts_w46_testov_300um_all.root";
# 	eu_cut_file="eu_cuts_w46_testov_1000um_run_2400.root";
# 	ga_lut_file="LUT_GALILEO_w46_Testov.dat";
# 	
# 	if [ "$runnb" -eq "2400" ]; then 	   
# 	   eu_cut_file="eu_cuts_w46_testov_1000um_run_2400.root";
#            eu_lut_file="LUT_EUCLIDES_w46_run_2400.dat";
# 	fi
# 	
# 	if [ "$runnb" -eq "2401" ]; then 	   
# 	   eu_cut_file="eu_cuts_w46_testov_1000um_run_2400.root";
#            eu_lut_file="LUT_EUCLIDES_w46_run_2400.dat";
# 	fi
# 	
# 	if [ "$runnb" -eq "2402" ]; then 	   
# 	   eu_cut_file="eu_cuts_w46_testov_1000um_run_2402.root";
#            eu_lut_file="LUT_EUCLIDES_w46_run_2402.dat";
# 	fi
# 	#runs 2403 2404 bad
# 	if [ "$runnb" -eq "2405" ]; then 	   
# 	   eu_cut_file="eu_cuts_w46_testov_1000um_run_2402.root";
#            eu_lut_file="LUT_EUCLIDES_w46_run_2405.dat";
# 	fi
# 	
# 	if [ "$runnb" -eq "2406" ]; then 	   
# 	   eu_cut_file="eu_cuts_w46_testov_1000um_run_2402.root";
#            eu_lut_file="LUT_EUCLIDES_w46_run_2406.dat";
# 	fi	
# 	FINISHED=0;
# 	;;
# ##########################################################################################################		
# "1500")	if [ "$FIRST" -eq "0" ]; then 
# 	   FIRST=2500 LAST=2503 #run 2501 is bad
# 	fi
# 	
# 	echo "Will run the selector for run mass $MASS for runs from $FIRST up to $LAST"	
# 	
# 	runnb=$FIRST
# 	while test $runnb -le $LAST
# 	do
# 	
# # 	eu_lut_file="LUT_EUCLIDES_w46_Testov.dat";
# # 	eu_cut_file="euclides_cuts_w46_testov_300um_all.root";
#         eu_lut_file="LUT_EUCLIDES_w46_run_2500.dat";
#         eu_cut_file="eu_cuts_w46_testov_1000um_run_2400.root";
# 	ga_lut_file="LUT_GALILEO_w46_Testov.dat";
# 	
# 	if [ "$runnb" -eq "2500" ]; then 	   
#            eu_lut_file="LUT_EUCLIDES_w46_run_2500_Testov.dat";
#            eu_cut_file="eu_cuts_w46_testov_1500um_run_2500.root";
# 	fi
# 	
# 	if [ "$runnb" -eq "2502" ]; then 
#  	   eu_lut_file="LUT_EUCLIDES_w46_run_2502_Testov.dat";
#  	   eu_cut_file="eu_cuts_w46_testov_1000um_run_2400.root";
# #	   eu_lut_file="LUT_EUCLIDES_w46_Testov_zero_time.dat";	
# 	fi	
# 	
# 	
# 	if [ "$runnb" -eq "2503" ]; then 
#  	   eu_lut_file="LUT_EUCLIDES_w46_run_2503_Testov.dat";
#  	   eu_cut_file="eu_cuts_w46_testov_1000um_run_2400.root";
# #	   eu_lut_file="LUT_EUCLIDES_w46_Testov_zero_time.dat";	
# 	fi	
# 	
# 	echo "--------------------------------------------------------"
# 	echo "Distance is set to $DIST um file [$file_prefix$runnb.root"]
# 	echo "--------------------------------------------------------"	
# 	echo -ne "LUT file:: "; tput setaf 2; echo " $ga_lut_file";tput sgr0;
# 	echo -ne "LUT file:: "; tput setaf 2; echo " $eu_lut_file";tput sgr0;
# 	echo -ne "CUT file:: "; tput setaf 2; echo " $eu_cut_file";tput sgr0;
# 	echo -ne "DATA file  "; tput setaf 6; echo " $data_path$file_prefix$runnb.root";tput sgr0;
# 	echo "LUT dir::   $lut_path"
# 	echo "CUT dir::   $cut_path"
# 	if [ "$TEST" -eq "1" ]; then 
# 
# 	tput setaf 1; echo "Testing: 1000000 events...."; tput sgr0;    
# 	fi
# 	echo "--------------------------------------------------------"
# 
# 
# 	#echo "$cut_path""cut_charged_particles.root"
# 	#echo "$cut_path""$eu_cut_file" "$cut_path""cut_charged_particles.root"
# 	unlink "$cut_path""cut_charged_particles.root"
# 	ln -s "$cut_path$eu_cut_file" "$cut_path""cut_charged_particles.root"
# 
# 	unlink "$lut_path""LUT_EUCLIDES.dat"
# 	ln -s "$lut_path$eu_lut_file" "$lut_path""LUT_EUCLIDES.dat"
# 
# 	unlink "$lut_path""LUT_GALILEO.dat"
# 	ln -s "$lut_path$ga_lut_file" "$lut_path""LUT_GALILEO.dat"	
# 	
#         if test -f "$data_path$file_prefix$runnb.root"
# 	    then	
# 		echo $rootcommand
# 		echo starting "$inputpath/$prefix$runnb.root"        
# 		rootcommand=automated_selector.C"($runnb,$runnb,$DIST,$TEST)" 
# 		echo "rootcommand $rootcommand"
# 		if [ $FLAG -eq 1 ]; then	    
# 		    root -l -b -q $rootcommand
# 		    else 
# 		    echo "<<<bash-script test>>"
# 		fi
# 	    fi
# 	    runnb=$(($runnb + 1))
# 	done
# 	FINISHED=1;
# 	;;
# ##########################################################################################################	
# #Decembre 2016
# ##########################################################################################################	
# "101")	if [ "$FIRST" -eq "0" ]; then  #100 um Decembre 2016
# 	   FIRST=3100 LAST=3118
# 	fi
# 	
# 	echo "Will run the selector for run mass $MASS for runs from $FIRST up to $LAST"	
# 	
# 	runnb=$FIRST
# 	while test $runnb -le $LAST
# 	do
# 	BADFILE=0;
#         eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3103.dat";
# 	#eu_cut_file="euclides_cuts_w46_testov_300um_all.root";
# 	eu_cut_file="euclides_cuts_run_3200_700um.root";
# 	ga_lut_file="LUT_GALILEO_w49_Testov_new.dat";
# 	
# 	if [ "$runnb" -eq "3102" ]; then
# 	   BADFILE=1;
#  	   eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3102.dat";   
# 	   #eu_lut_file="LUT_EUCLIDES_w49_Testov_zero_time.dat";	
# 	fi
# 	
# 	if [ "$runnb" -eq "3105" ]; then 
#  	   eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3105.dat";   
# 	   #eu_lut_file="LUT_EUCLIDES_w49_Testov_zero_time.dat";	
# 	fi	
# 	
# 	if [ "$runnb" -eq "3106" ]; then 
#  	   eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3106.dat";   
# 	   #eu_lut_file="LUT_EUCLIDES_w49_Testov_zero_time.dat";	
# 	fi
# 	
# 	if [ "$runnb" -eq "3109" ]; then 
#  	   eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3109.dat";   
# 	   #eu_lut_file="LUT_EUCLIDES_w49_Testov_zero_time.dat";	
# 	fi
# 	
# 	if [ "$runnb" -eq "3110" ]; then 
#  	   eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3109.dat";   
# 	   #eu_lut_file="LUT_EUCLIDES_w49_Testov_zero_time.dat";	
# 	fi
# 	
# 	if [ "$runnb" -eq "3111" ]; then 
#  	   eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3109.dat";   
# 	   #eu_lut_file="LUT_EUCLIDES_w49_Testov_zero_time.dat";	
# 	fi
# 	
# 	if [ "$runnb" -eq "3112" ]; then 
#  	   eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3109.dat";   
# 	   #eu_lut_file="LUT_EUCLIDES_w49_Testov_zero_time.dat";	
# 	fi
# 	
# 	if [ "$runnb" -eq "3113" ]; then 
#  	   eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3113.dat";   
# 	   #eu_lut_file="LUT_EUCLIDES_w49_Testov_zero_time.dat";	
# 	fi
# 	
# 	if [ "$runnb" -eq "3114" ]; then 
#  	   eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3114.dat";   
# 	   #eu_lut_file="LUT_EUCLIDES_w49_Testov_zero_time.dat";	
# 	fi
# 	
# 	if [ "$runnb" -ge "3115" ]; then 
#  	   eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3117.dat";   
# 	   #eu_lut_file="LUT_EUCLIDES_w49_Testov_zero_time.dat";	
# 	fi	
# 	
# 	echo "--------------------------------------------------------"
# 	echo "Distance is set to $DIST um file [$file_prefix$runnb.root"]
# 	echo "--------------------------------------------------------"	
# 	echo -ne "LUT file:: "; tput setaf 2; echo " $ga_lut_file";tput sgr0;
# 	echo -ne "LUT file:: "; tput setaf 2; echo " $eu_lut_file";tput sgr0;
# 	echo -ne "CUT file:: "; tput setaf 2; echo " $eu_cut_file";tput sgr0;
# 	echo -ne "DATA file  "; tput setaf 6; echo " $data_path$file_prefix$runnb.root";tput sgr0;
# 	echo "LUT dir::   $lut_path"
# 	echo "CUT dir::   $cut_path"
# 	if [ "$TEST" -eq "1" ]; then 
# 
# 	tput setaf 1; echo "Testing: 1000000 events...."; tput sgr0;    
# 	fi
# 	echo "--------------------------------------------------------"
# 	
# 	#if [ "$BADFILE" -eq "1" ]; then 
# 	#    continue
# 	#fi
# 
# 	#echo "$cut_path""cut_charged_particles.root"
# 	#echo "$cut_path""$eu_cut_file" "$cut_path""cut_charged_particles.root"
# 	unlink "$cut_path""cut_charged_particles.root"
# 	ln -s "$cut_path$eu_cut_file" "$cut_path""cut_charged_particles.root"
# 
# 	unlink "$lut_path""LUT_EUCLIDES.dat"
# 	ln -s "$lut_path$eu_lut_file" "$lut_path""LUT_EUCLIDES.dat"
# 
# 	unlink "$lut_path""LUT_GALILEO.dat"
# 	ln -s "$lut_path$ga_lut_file" "$lut_path""LUT_GALILEO.dat"	
# 	
#         if test -f "$data_path$file_prefix$runnb.root"
# 	    then	
# 		echo $rootcommand
# 		echo starting "$inputpath/$prefix$runnb.root"        
# 		rootcommand=automated_selector.C"($runnb,$runnb,$DIST,$TEST)" 
# 		echo "rootcommand $rootcommand"
# 		if [ $FLAG -eq 1 ]; then	    
# 		    root -l -b -q $rootcommand
# 		    else 
# 		    echo "<<<bash-script test>>"
# 		fi
# 	    fi
# 	    runnb=$(($runnb + 1))
# 	done
# 	FINISHED=1;
# 	;;
# ##########################################################################################################		
# "700")	if [ "$FIRST" -eq "0" ]; then  
# 	   FIRST=3201 LAST=3203
# 	fi
# 	eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3106.dat";
# 	eu_cut_file="euclides_cuts_run_3200_700um.root";
# 	ga_lut_file="LUT_GALILEO_w49_Testov_new.dat";
# 	
# 	if [ "$runnb" -eq "3202" ]; then 
#  	   eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3203.dat";   	   
# 	fi	
# 	
# 	if [ "$runnb" -eq "3203" ]; then 
#  	   eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3203.dat";   	   
# 	fi
# 	
# 	FINISHED=0;
# 	;;
# ##########################################################################################################	
# #Blackout
# ##########################################################################################################
# "10")	if [ "$FIRST" -eq "0" ]; then  
# 	   FIRST=3400 LAST=3408
# 	fi
# 	#eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3400.dat";
# 	eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3900.dat";
# 	eu_cut_file="euclides_cuts_w46_testov_300um_all.root";
# 	ga_lut_file="LUT_GALILEO_w49_Testov_run_3400.dat";
# 	FINISHED=0;
# 	;;
# ##########################################################################################################
# "60")	if [ "$FIRST" -eq "0" ]; then  
# 	   FIRST=3500 LAST=3505
# 	fi
# 	#eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3400.dat";
# 	eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3900.dat";
# 	eu_cut_file="euclides_cuts_w46_testov_300um_all.root";
# 	ga_lut_file="LUT_GALILEO_w49_Testov_run_3400.dat";
# 	FINISHED=0;
# 	;;
# ##########################################################################################################
# "150")	if [ "$FIRST" -eq "0" ]; then  
# 	   FIRST=3600 LAST=3605
# 	fi
# 	#eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3400.dat";
# 	eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3900.dat";
# 	eu_cut_file="euclides_cuts_w46_testov_300um_all.root";
# 	ga_lut_file="LUT_GALILEO_w49_Testov_run_3400.dat";
# 	FINISHED=0;
# 	;;
# ##########################################################################################################
# "18")	if [ "$FIRST" -eq "0" ]; then  
# 	   FIRST=3700 LAST=3702
# 	fi
# 	#eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3400.dat";
# 	eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3400.dat";
# 	eu_cut_file="euclides_cuts_w46_testov_300um_all.root";
# 	ga_lut_file="LUT_GALILEO_w49_Testov_run_3400.dat";
# 	FINISHED=0;
# 	;;
# ##########################################################################################################
# "2000")	if [ "$FIRST" -eq "0" ]; then  
# 	   FIRST=3800 LAST=3803
# 	fi
# 	#eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3400.dat";
# 	eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3900.dat";   	   
# 	eu_cut_file="euclides_cuts_run_3900_130um.root";
# 	ga_lut_file="LUT_GALILEO_w49_Testov_run_3400.dat";
# 	FINISHED=0;
# 	;;
# ##########################################################################################################
# "130")	if [ "$FIRST" -eq "0" ]; then  
# 	   FIRST=3900 LAST=3904
# 	fi		
# 	
# 	eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3900.dat";
# 	
# # 	eu_cut_file="euclides_cuts_w46_testov_300um_all.root";
#  	eu_cut_file="euclides_cuts_run_3900_130um.root";
#  	
# 	ga_lut_file="LUT_GALILEO_w49_Testov_run_3400.dat";
# 	
# 	
# 	if [ "$runnb" -ge "3900" ]; then 
#  	   eu_lut_file="LUT_EUCLIDES_w49_Testov_run_3900.dat";   	   
# 	fi	
# 	
# 	
# 	FINISHED=0;
# 	;;
# esac
# 
# if [ "$FINISHED" -eq "0" ]; then 
# 
# echo "Will run the selector for run mass $MASS for runs from $FIRST up to $LAST"	
# 	
# 	runnb=$FIRST
# 	while test $runnb -le $LAST
# 	do	
# 	
# 	echo "--------------------------------------------------------"
# 	echo "Distance is set to $DIST um file [$file_prefix$runnb.root"]
# 	echo "--------------------------------------------------------"	
# 	echo -ne "LUT file:: "; tput setaf 2; echo " $ga_lut_file";tput sgr0;
# 	echo -ne "LUT file:: "; tput setaf 2; echo " $eu_lut_file";tput sgr0;
# 	echo -ne "CUT file:: "; tput setaf 2; echo " $eu_cut_file";tput sgr0;
# 	echo -ne "DATA file  "; tput setaf 6; echo " $data_path$file_prefix$runnb.root";tput sgr0;
# 	echo "LUT dir::   $lut_path"
# 	echo "CUT dir::   $cut_path"
# 	if [ "$TEST" -eq "1" ]; then 
# 
# 	tput setaf 1; echo "Testing: 1000000 events...."; tput sgr0;    
# 	fi
# 	echo "--------------------------------------------------------"
# 	
# 	unlink "$cut_path""cut_charged_particles.root"
# 	ln -s "$cut_path$eu_cut_file" "$cut_path""cut_charged_particles.root"
# 
# 	unlink "$lut_path""LUT_EUCLIDES.dat"
# 	ln -s "$lut_path$eu_lut_file" "$lut_path""LUT_EUCLIDES.dat"
# 
# 	unlink "$lut_path""LUT_GALILEO.dat"
# 	ln -s "$lut_path$ga_lut_file" "$lut_path""LUT_GALILEO.dat"	
# 	
#         if test -f "$data_path$file_prefix$runnb.root"
# 	    then	
# 		echo $rootcommand
# 		echo starting "$inputpath/$prefix$runnb.root"        
# 		rootcommand=automated_selector.C"($runnb,$runnb,$DIST,$TEST)" 
# 		echo "rootcommand $rootcommand"
# 		if [ $FLAG -eq 1 ]; then	    
# 		    root -l -b -q $rootcommand
# 		    else 
# 		    echo "<<<bash-script test>>"
# 		fi
# 	    fi
# 	    runnb=$(($runnb + 1))
# 	done
# 	FINISHED=1;
# fi
# 
# 
# 
# 
# 
