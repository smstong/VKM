all: vkm

vkm: *.c
	gcc -o $@ $^

install: vkm vkm.service
	sudo cp -f vkm /usr/local/bin/
	sudo cp -f vkm.service /etc/systemd/system/
	sudo firewall-cmd --add-port=8888/udp --permanent
	sudo firewall-cmd --reload

clean:
	rm -f vkm
		
	
