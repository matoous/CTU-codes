
#TOKEN:HystericalSpaghettiFlossApparently

declare TOKEN_FILE=/var/tmp/tokens/second_token.txt;
declare tmp_file=/home/user/assignment07/.token_id;
declare -a TOKENS=("Hysterical" "Spaghetti" "Floss" "Apparently");
#declare -a PASSWORDS=("professor" "scarlet" "detective" "4a7#mgannn2LDD90T#1fX#0Yx%m!kxrMSmUXd60xKwdM0S6u");
declare -a PASSWORDS=("superman" "ironman" "batman" "4a7#mgannn2LDD90T#1fX#0Yx%m!kxrMSmUXd60xKwdM0S6u");
id=0;

#read current index from file
while IFS= read -r line;do
	id=$line;
done < $tmp_file

#increment it (or rotate to 0)
if [[ $id -eq "3" ]]; then
	id=0;
else
	((id++));
fi


#kill all ssh @moriarty
for i in $(who -u | grep moriarty | awk -F " " '{print $5}')
do
#	echo $i;
	kill $i;
done

# Override the token file
#add server info if this is the last part
if [[ $id -eq "3" ]]; then
	echo "$(($id +1))/4: ${TOKENS[$id]}" > $TOKEN_FILE;
	echo "Flag server: 192.168.1.167:9453" >> $TOKEN_FILE;
else
	echo "$(($id +1))/4: ${TOKENS[$id]}" > $TOKEN_FILE;
fi

#change password
echo -e "moriarty:${PASSWORDS[$id]}" | /usr/sbin/chpasswd;

#save the current index
echo $id > $tmp_file;