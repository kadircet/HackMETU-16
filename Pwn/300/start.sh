chmod -R 700 .

for i in `seq 0 $(($1-1))`
do
	gcc team$i/pwn300.c -oteam$i/p300 -ldl -fstack-protector-all
	useradd -M p300t$i -s/usr/sbin/nologin
	chown -R p300t$i.p300t$i team$i
	chmod 700 -R team$i
	cd team$i
	echo p300 $i
	sudo -u p300t$i ./p300 &
	cd ..
done
