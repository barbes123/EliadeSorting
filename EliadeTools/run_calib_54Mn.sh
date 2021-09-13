#!/bin/bash
InputSource=$1
FIRSTdomain=$2
LASTdomain=$3
# sources: 60Co 54Mn
source=$InputSource

lim1seg=500
lim2seg=800
fwhmseg=3
amplseg=10

lim1core=300
lim2core=500
fwhmcore=1.3
amplcore=100

addK60="no"


CUR_DIR=$(pwd)
cd $CUR_DIR

echo "I will do calibratation for $source source. Take a cofee relux "

echo "CUR_DIR $CUR_DIR"


domnb=$FIRSTdomain
echo "mEliade_raw_py_$domnb.spe" 
 if test -f "mEliade_raw_py_$domnb.spe" 
   then
   echo "yessssssssssss"
   else
   echo "nooooooooooooooooo"
 fi

echo "Will run the calib for domains $FIRSTdomain upto $LASTdomain "

##################################   
 if test -f "eliade.calib" 
   then 
   	rm "eliade.calib"   
   fi
##################################   
    if test -f "source.calib" 
   then 
   	rm "source.calib"   
   fi
##################################      
    if test -f "source.temp" 
   then 
   	rm "source.temp"   
   fi
##################################      
    if test -f "temp.calib" 
   then 
   	rm "temp.calib"   
   fi
##################################      
   
   




domnb=$FIRSTdomain
while test $domnb -le $LASTdomain
do
 echo "Now starting calib for domain $domnb"	
 
 if [[ "$domnb" == "101" ]] ||  [[ "$domnb" == "111" ]]||  [[ "$domnb" == "121" ]]||  [[ "$domnb" == "131" ]];
 then
  lim1=$lim1core
  lim2=$lim2core
  fwhm=$fwhmcore
  ampl=$amplcore
  echo "changed for core $domnb"
 else
  lim1=$lim1seg
  lim2=$lim2seg
  fwhm=$fwhmseg
  ampl=$amplseg
  echo "changed for segment $domnb"
 fi
 
 if test -f "mEliade_raw_py_$domnb.spe" 
   then
      #get full data on calibration
    if [  $source = '54Mn' ]
       then
       #echo "$source is running"       
       src="ener 834.848"
      else      
       #echo "$source is not running PROBLEM"
       src=$source
      fi   
    echo "Source is $source, parameters $src"       
      
      
      if [  $addK60 = 'yes' ]
      then
        echo "Adding 60K"
        echo "/data/live/IT/tools/RecalEnergy -spe mEliade_raw_py_$domnb.spe -fmt A 16384 -lim $lim1 $lim2 -dwa $fwhm $ampl -$src -v 2 -poly1 -ener 1460.82"
       /data/live/IT/tools/RecalEnergy -spe mEliade_raw_py_$domnb.spe -fmt A 16384 -lim $lim1 $lim2 -dwa $fwhm $ampl -$src -v 2 -poly1 -ener 1460.82
       /data/live/IT/tools/RecalEnergy -spe mEliade_raw_py_$domnb.spe -fmt A 16384 -lim $lim1 $lim2 -dwa $fwhm $ampl -$src -v 2 -poly1 -ener 1460.82 > fulldata.calib
       else
       echo "/data/live/IT/tools/RecalEnergy -spe mEliade_raw_py_$domnb.spe -fmt A 16384 -lim $lim1 $lim2 -dwa $fwhm $ampl -$src -v 2 -poly1"
       /data/live/IT/tools/RecalEnergy -spe mEliade_raw_py_$domnb.spe -fmt A 16384 -lim $lim1 $lim2 -dwa $fwhm $ampl -$src -v 2 -poly1
       /data/live/IT/tools/RecalEnergy -spe mEliade_raw_py_$domnb.spe -fmt A 16384 -lim $lim1 $lim2 -dwa $fwhm $ampl -$src -v 2 -poly1 > fulldata.calib
      fi 
      
      #delete first 14 lines
      awk 'NR > 14 { print }' fulldata.calib >  temp.calib
      
      echo -n "domain $domnb " >> eliade.calib      

      
	
	
	if [ $source = '60Co' ]
	then
	        #############60Co#################      
	        if grep 'Cal1' temp.calib
		then
	 	   grep 'Cal1'  temp.calib >> eliade.calib      
	 	   echo "Found Cal1"
	 	else echo " ">> eliade.calib #if domain is emtry add a new line
		fi 
		if grep '1173.238' temp.calib
		then
		   #echo -n "domain $domnb " >> source.calib    
	 	   grep '1173.238'  temp.calib >> source.calib      
	 	   echo "Found 1173.238"
		fi
		if grep '1332.513' temp.calib	    
		then
		   #echo -n "domain $domnb " >> source.calib 
	 	   grep '1332.513'  temp.calib >> source.temp      
	 	   echo "Found"
		fi
		if grep '#2' source.temp
		then
 	   	   grep '#2'  source.temp >> source.calib      
 	   	   echo "Found"
		fi
	fi
	if [ $source = '54Mn' ]
	then	
		#############54Mn#################            
		if grep 'Slope =  ' temp.calib
		then
	 	   grep 'Slope =  '  temp.calib >> eliade.calib      
	 	   echo "Found Slope =  "
	 	else echo " ">> eliade.calib #if domain is emtry add a new line
		fi 
		if grep '834.85' temp.calib
		then
		   #echo -n "domain $domnb " >> source.calib    
	 	   grep '834.85'  temp.calib >> source.temp      
	 	   echo "Found 834.85"
		fi
               echo -n "$domnb " >> source.calib      
		grep '834.85'  temp.calib >> source.calib      
	fi	
	##################################
	if grep '#2' source.temp
	then
 	   grep '#2'  source.temp >> source.calib      
 	   echo "Found"
	fi            
   else 
     echo no file "mEliade_raw_py_$domnb.spe" 
     echo "domain $domnb #2     0  Slope = 0.000000    Cal1=[ -0.0000  1.000000 ]" >> eliade.calib       
     #echo "#2" >> eliade.calib 
     #sed -i "s/#2/$domnb/" eliade.calib
   fi  

sed -i "s/#2/$domnb/" source.calib

 rm source.temp
 rm temp.calib	
 domnb=$(($domnb + 1))
done 
 
cat eliade.calib | sed 's/|/ /' | awk '{print $2, $9, $10}' >  eliade.coeff


if [ $source = '54Mn' ]
then	
 awk -F " " '{ print $1 " " $5 " " $6 " " $10 " " $11 " " $11/$10*$6 " keV"}' source.calib > resolution.txt
else 
 aawk -F " " '{ print $1 " " $5 " " $9 " " $6 " " $7 " " $7/$6*$9 " keV"}' source.calib > resolution.txt
fi


