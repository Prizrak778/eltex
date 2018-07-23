#!/bin/bash
ip_addr=`echo $1 |tr -s '.' '\n'`
netmask=`echo $2 |tr -s '.' '\n'`
ip_addr_bin=$(echo "obase=2; ibase=10; $ip_addr"|bc |tr -s '\n' '.')
ip_addr_bin=$(echo $ip_addr_bin | sed 's/.$//')
len=`echo "$ip_addr_bin" | wc -c`
netmask_bin=$(echo "obase=2; ibase=10; $netmask"|bc |tr -s '\n' '.')
netmask_bin=$(echo $netmask_bin | sed 's/.$//')
for tmp in $(echo "$ip_addr_bin"|tr -s '.' '\n')
do
	len=`echo "$tmp" |wc -c`
	let len=9-$len
	for (( j = 0; j < len; j+=1))
	do
		tmp=0$tmp
	done
	ip_addr_bin_full+="$tmp."
	ip_addr_dec_full+=$tmp
done
for tmp in $(echo "$netmask_bin"|tr -s '.' '\n')
do
	len=`echo "$tmp" |wc -c`
	let len=9-$len
	for (( j = 0; j < len; j+=1))
	do
		tmp=0$tmp
	done
	netmask_bin_full+="$tmp."
	ip_addr_bin_wildcard+=$tmp
	netmask_dec_full+=$tmp
done
ip_addr_bin_full=$(echo $ip_addr_bin_full | sed 's/.$//')
ip_addr_bin_wildcard=$(echo "obase=10; ibase=2; $ip_addr_bin_wildcard" | bc)
netmask_bin_full=$(echo $netmask_bin_full | sed 's/.$//')
echo -e "Address \e[34m$1\e[0m\t\t \e[33m$ip_addr_bin_full\e[0m"
let 'ip_addr_bin_wildcard=4294967295-ip_addr_bin_wildcard'
let 'hostnet=ip_addr_bin_wildcard-1'
ip_addr_bin_wildcard=$(echo "obase=2; ibase=10; $ip_addr_bin_wildcard"|bc)
len_mask=$(echo $netmask_bin_full| grep -o 1| wc -l)
echo -e "Netmask: \e[34m$2 = $len_mask\e[0m\t \e[31m$netmask_bin_full\e[0m"
len_wild=`echo "$ip_addr_bin_wildcard"| wc -c`
let len_wild=$len_wild-1
for (( i=0; i<32-len_wild; i++))
do
	ip_addr_bin_wildcard=0$ip_addr_bin_wildcard
done
wildcard_dec=$(echo $ip_addr_bin_wildcard| sed -e :a -e 's/\(.*[0-1]\)\([0-1]\{8\}\)/\1\n\2/;ta')
wildcard_dec=$(echo "obase=10; ibase=2; $wildcard_dec" |bc |tr -s '\n' '.')
wildcard=$(echo $wildcard_dec | sed 's/.$//')
ip_addr_bin_wildcard=$(echo $ip_addr_bin_wildcard| sed -e :a -e 's/\(.*[0-1]\)\([0-1]\{8\}\)/\1.\2/;ta')
echo -e "Wildcard: \e[34m$wildcard\e[0m\t\t \e[33m$ip_addr_bin_wildcard\e[0m"
echo "=>"
ip_addr_dec_full=$(echo "obase=10; ibase=2; $ip_addr_dec_full"|bc)
netmask_dec_full=$(echo "obase=10; ibase=2; $netmask_dec_full"|bc)
let 'network=ip_addr_dec_full & netmask_dec_full'
let 'network_dec=network-1'
network_dec=$(echo "obase=2; ibase=10; $network_dec"|bc)
network_dec=$(echo $network_dec|sed -e :a -e 's/\(.*[0-1]\)\([0-1]\{8\}\)/\1\n\2/;ta')
network_dec=$(echo "obase=10; ibase=2; $network_dec"|bc |tr '\n' '.'| sed 's/.$//')
let hostmin=$network+1
hostmin_dec=$(echo "obase=2; ibase=10; $hostmin"|bc)
hostmin_dec=$(echo $hostmin_dec|sed -e :a -e 's/\(.*[0-1]\)\([0-1]\{8\}\)/\1\n\2/;ta')
hostmin_dec=$(echo "obase=10; ibase=2; $hostmin_dec"|bc |tr '\n' '.' | sed 's/.$//')
let hostmax=$network+2**len_wild-2
hostmax_dec=$(echo "obase=2; ibase=10; $hostmax"|bc)
hostmax_dec=$(echo $hostmax_dec|sed -e :a -e 's/\(.*[0-1]\)\([0-1]\{8\}\)/\1\n\2/;ta')
hostmax_dec=$(echo "obase=10; ibase=2; $hostmax_dec"|bc |tr '\n' '.' | sed 's/.$//')
let broadcast=$hostmax+1
broadcast_dec=$(echo "obase=2; ibase=10; $broadcast"|bc)
broadcast_dec=$(echo $broadcast_dec|sed -e :a -e 's/\(.*[0-1]\)\([0-1]\{8\}\)/\1\n\2/;ta')
broadcast_dec=$(echo "obase=10; ibase=2; $broadcast_dec"|bc |tr '\n' '.' | sed 's/.$//')
network=$(echo "obase=2; ibase=10; $network"|bc)
network=$(echo $network| sed -e :a -e 's/\(.*[0-1]\)\([0-1]\{8\}\)/\1.\2/;ta')
echo -e "Network: \e[34m$network_dec/"$len_mask"\e[0m\t \e[33m$network\e[0m"
hostmin=$(echo "obase=2; ibase=10; $hostmin"|bc)
hostmin=$(echo $hostmin| sed -e :a -e 's/\(.*[0-1]\)\([0-1]\{8\}\)/\1.\2/;ta')
echo -e "Hostmin: \e[34m$hostmin_dec\e[0m\t\t \e[33m$hostmin\e[0m"
hostmax=$(echo "obase=2; ibase=10; $hostmax"|bc)
hostmax=$(echo $hostmax| sed -e :a -e 's/\(.*[0-1]\)\([0-1]\{8\}\)/\1.\2/;ta')
echo -e "Hostmax: \e[34m$hostmax_dec\e[0m\t \e[33m$hostmax\e[0m"
broadcast=$(echo "obase=2; ibase=10; $broadcast"|bc)
broadcast=$(echo $broadcast| sed -e :a -e 's/\(.*[0-1]\)\([0-1]\{8\}\)/\1.\2/;ta')
echo -e "Broadcast: \e[34m$broadcast_dec\e[0m\t \e[33m$broadcast\e[0m"
echo -e "Hosts/Net: \e[34m$hostnet\e[0m"
exit

