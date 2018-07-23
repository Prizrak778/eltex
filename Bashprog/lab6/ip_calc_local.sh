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
	ip_addr_bin_dec_full+=$tmp
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
	netmask_bin_dec_full+=$tmp
done
ip_addr_bin_full=$(echo $ip_addr_bin_full | sed 's/.$//')
ip_addr_bin_wildcard=$(echo $ip_addr_bin_wildcard | sed 's/.$//')
netmask_bin_full=$(echo $netmask_bin_full | sed 's/.$//')
echo -e "Address \e[34m$1\e[0m\t\t \e[33m$ip_addr_bin_full\e[0m"
let 'ip_addr_bin_wildcard=4294967295-ip_addr_bin_wildcard'
let 'hostnet=ip_addr_bin_wildcard-1'
ip_addr_bin_wildcard=$(echo "obase=2; ibase=10; $ip_addr_bin_wildcard"|bc)
len_mask=$(echo $netmask_bin_full| grep -o 1| wc -l)
echo -e "Netmask: \e[34,$2 = $len_mask\e[0m\t \e[31m$netmask_bin_full\e[0m"
