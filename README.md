# OS_project1
    
note: 
for generating the program, use this command
make

for output to *stdout.txt, use this command 
sudo ./schedule < *.txt > output/*_stdout.txt

for getting input from dmesg, use this command
dmesg | grep Project1 | cut -b 16- > output/*_dmesg.txt

for cleaing the dmesg log, use this command
sudo dmesg -c


for convenience

sudo dmesg -c
sudo ./schedule < OS_PJ1_Test/TIME_MEASUREMENT.txt > output/TIME_MEASUREMENT_stdout.txt
dmesg | grep Project1 | cut -b 16- > output/TIME_MEASUREMENT_dmesg.txt

sudo dmesg -c
sudo ./schedule < OS_PJ1_Test/FIFO_1.txt > output/FIFO_1_stdout.txt
dmesg | grep Project1 | cut -b 16- > output/FIFO_1_dmesg.txt

sudo dmesg -c
sudo ./schedule < OS_PJ1_Test/PSJF_2.txt > output/PSJF_2_stdout.txt
dmesg | grep Project1 | cut -b 16- > output/PSJF_2_dmesg.txt

sudo dmesg -c
sudo ./schedule < OS_PJ1_Test/RR_3.txt > output/RR_3_stdout.txt
dmesg | grep Project1 | cut -b 16- > output/RR_3_dmesg.txt

sudo dmesg -c
sudo ./schedule < OS_PJ1_Test/SJF_4.txt > output/SJF_4_stdout.txt
dmesg | grep Project1 | cut -b 16- > output/SJF_4_dmesg.txt


